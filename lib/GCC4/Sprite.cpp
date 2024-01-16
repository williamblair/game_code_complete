#include <cassert>
#include "Sprite.h"
#include "Logger.h"
#include "OGLRenderer.h"
#include "OGLCreateTexture.h"
#include "OGLTextureResourceView.h"
#include "OGLTextureResourceExtraData.h"
#include "Resource.h"
#include "ResHandle.h"
#include "ResCache.h"

// static member variables
OGLShader* Sprite::s_pSpriteShader = nullptr;
OGLVertexBuffer* Sprite::s_SpriteVertBuf = nullptr;
unsigned int Sprite::s_SpriteCount = 0;

Sprite::Sprite()
{
    if (!s_pSpriteShader) {
        if (!InitSpriteShader()) {
          GCC_ERROR("Failed to init sprite shader");
          assert(false);
        }
        if (!InitSpriteVertBuf()) {
            GCC_ERROR("Failed to init sprite vertex buffer");
            assert(false);
        }
    }
    ++s_SpriteCount;
    m_Texture = 0;
}

Sprite::~Sprite()
{
    assert(s_SpriteCount > 0);
    --s_SpriteCount;
    if (s_SpriteCount == 0) {
        delete s_pSpriteShader;
        s_pSpriteShader = nullptr;
        delete s_SpriteVertBuf;
        s_SpriteVertBuf = nullptr;
    }
    if (m_Texture) {
        glDeleteTextures(1, &m_Texture);
    }
}

void Sprite::VOnUpdate(int deltaMs)
{
    //TODO
}

bool Sprite::VOnRender(float fTime, float fElapsedTime)
{
    OGLRenderer* pRndr = OGLRenderer::GetInstance();
    assert(pRndr);

    // TODO - adjust source rect for 2d sprite animatiion
    // Rect sourceRect(Point(0,0), Point(m_Width-1,m_Height-1));
    //sourceRect.top += m_CurrentFrame * m_TextureHeight;
    //sourceRect.bottom += m_CurrentFrame * m_TextureHeight;

    // scale from [0,width] to [-1..1], also flip y position
    Point actualPos(m_Position - m_Hotspot);
    printf("Actual Pos: %d,%d\n", actualPos.x, actualPos.y);
    Vec2 normPosOffset(
        -1.0f + (2.0f * (actualPos.x/float(pRndr->VGetWidth()))),
        -1.0f + (2.0f * ((pRndr->VGetHeight()-actualPos.y)/float(pRndr->VGetHeight())))
    );
    normPosOffset = normPosOffset - Vec2(-1.0f,1.0f); // starting point of the vertices
    // not *2 as vertices are already [-1..+1]
    Vec2 normScale(
        /*float(m_Width)/float(pRndr->VGetWidth()),
        float(m_Height)/float(pRndr->VGetHeight())*/
        1.0f,1.0f
    );

    printf("Norm pos offset: %f,%f\n", normPosOffset.x, normPosOffset.y);
    printf("Norm scale: %f,%f\n", normScale.x, normScale.y);

    Mat4x4 mvpMat =
        Translate(normPosOffset.x, normPosOffset.y, 0.0f) *
        Scale(normScale.x, normScale.y, 1.0f);
    pRndr->SetShader(*s_pSpriteShader);
    s_pSpriteShader->SetMat4("uMvpMatrix", mvpMat);
    s_pSpriteShader->SetVec4("uColor", Vec4(1.0f,1.0f,1.0f,1.0f));

    Vec3 tformPos = Transform(mvpMat, Vec3(-1.0f, 1.0f, 0.0f));
    printf("tformPos: %f,%f,%f\n",
        tformPos.x, tformPos.y, tformPos.z);

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(s_SpriteVertBuf->mVAO);
    glDrawElements(GL_TRIANGLES, s_SpriteVertBuf->mNumIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    return true;
}



BitmapSprite::BitmapSprite(const std::string& fileName, bool inCache, const int frames)
{
    Resource resource(fileName);
    std::shared_ptr<ResHandle> pResHandle = g_pResCache->GetHandle(&resource);
    auto pExtra = std::static_pointer_cast<OGLTextureResourceExtraData>(
        pResHandle->GetExtra()
    );
    if (!pExtra) {
        GCC_ERROR("BitmapSprite failed to load texture");
        assert(false);
    }
    m_Texture = OGLCreateTexture(
        pExtra->GetTexture()->GetData(),
        pExtra->GetTexture()->GetWidth(), pExtra->GetTexture()->GetHeight(),
        pExtra->GetTexture()->GetBytesPerPixel()
    );
    m_TextureWidth = m_Width = pExtra->GetTexture()->GetWidth();
    m_TextureHeight = m_Height = pExtra->GetTexture()->GetHeight();
    m_NumFrames = frames;
}




TextSprite::TextSprite(
    const std::string& text,
    TextStyle style,
    bool isCentered,
    const Point& position,
    Point* m_pSize)
{
    //TODO
}

bool TextSprite::VOnRender(float fTime, float fElapsedTime)
{
    //TODO
    return true;
}

void TextSprite::SetText(const std::string text)
{
    //TODO
}

bool Sprite::InitSpriteShader()
{
    const char* vertexStr =
    "#version 330 core                                      \n"
    "                                                       \n"
    "layout (location = 0) in vec2 aPos;                    \n"
    "layout (location = 1) in vec2 aTexCoord;               \n"
    "                                                       \n"
    "out vec2 texCoord;                                     \n"
    "                                                       \n"
    "uniform mat4 uMvpMatrix;                               \n"
    "                                                       \n"
    "void main()                                            \n"
    "{                                                      \n"
    "    gl_Position = uMvpMatrix * vec4(aPos, 0.0, 1.0);   \n"
    "    texCoord = aTexCoord;                              \n"
    "}                                                      \n";
    const char* fragmentStr =
    "#version 330 core                                      \n"
    "                                                       \n"
    "out vec4 FragColor;                                    \n"
    "                                                       \n"
    "uniform sampler2D uTexture0;                           \n"
    "uniform vec4 uColor;                                   \n"
    "                                                       \n"
    "in vec2 texCoord;                                      \n"
    "                                                       \n"
    "void main()                                            \n"
    "{                                                      \n"
    "    FragColor = uColor * texture(uTexture0, texCoord); \n"
    "}                                                      \n";
    s_pSpriteShader = new OGLShader();
    return s_pSpriteShader->LoadFromString(vertexStr, fragmentStr);
}

bool Sprite::InitSpriteVertBuf()
{
    s_SpriteVertBuf = new OGLVertexBuffer();
    float vertices[4*4] = {
        // pos      tex coord
        -1.0f,1.0f,  0.0f,1.0f, // top left
        -1.0f,-1.0f, 0.0f,0.0f, // bottom left
        1.0f,-1.0f, 1.0f,0.0f, // bottom Right
        1.0f,1.0f,  1.0f,1.0f // top right
    };
    uint32_t indices[3*2] = {
        1,2,3, // bottom left, bottom right, top right
        1,3,0 // bottom left, top right, top left
    };
    glGenVertexArrays(1, &s_SpriteVertBuf->mVAO);
    glGenBuffers(1, &s_SpriteVertBuf->mVBO);
    glGenBuffers(1, &s_SpriteVertBuf->mEBO);
    glBindVertexArray(s_SpriteVertBuf->mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_SpriteVertBuf->mVBO);
    glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_SpriteVertBuf->mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint32_t), indices, GL_STATIC_DRAW);
    // aPos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // aTexCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    s_SpriteVertBuf->mNumVerts = 4;
    s_SpriteVertBuf->mNumIndices = 3*2;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}
