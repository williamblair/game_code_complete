#include <stdexcept>
#include "OGLSkyNode.h"
#include "OGLRenderer.h"
#include "Scene.h"
#include "OGLTextureResourceExtraData.h"
#include "OGLTextureResourceView.h"
#include "ResHandle.h"
#include "ResCache.h"

// https://learnopengl.com/Advanced-OpenGL/Cubemaps

OGLSkyNode::OGLSkyNode(const char* textureFile, std::shared_ptr<CameraNode> camera) :
    SceneNode(INVALID_ACTOR_ID, "Sky", RenderPass_Sky, g_White, &Mat4x4::g_Identity) ,
    m_TextureBaseName(textureFile),
    m_Camera(camera),
    m_bActive(true),
    m_CubeTexId(0)
{
    // Load the vertex buffer
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    if (!m_VertBuf.Init(OGLVertexBuffer::POS, skyboxVertices, 6*6, nullptr, 0)) {
        throw std::runtime_error("Failed to init sky node vertex buffer");
    }

    // Load the texture
    glGenTextures(1, &m_CubeTexId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTexId);
    std::string faces[6] = {
        m_TextureBaseName + "_right.jpg", // right
        m_TextureBaseName + "_left.jpg", // left
        m_TextureBaseName + "_top.jpg", // top
        m_TextureBaseName + "_bottom.jpg", // bottom
        m_TextureBaseName + "_front.jpg", // front
        m_TextureBaseName + "_back.jpg" // back
    };
    for (int i=0; i<6; ++i) {
        Resource resource(faces[i]);
        std::shared_ptr<ResHandle> pResHandle = g_pResCache->GetHandle(&resource);
        if (!pResHandle) { throw std::runtime_error("Failed to load " + faces[i]); }
        auto extraData = std::static_pointer_cast<OGLTextureResourceExtraData>(pResHandle->GetExtra());
        OGLTextureResourceView* pTex = extraData->GetTexture();
        uint8_t* pData = pTex->GetData();
        if (pData) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, pTex->GetWidth(), pTex->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
        } else {
            throw std::runtime_error("Failed to load sky box texture: " + faces[i]);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Load the shader
    const char* vertexShaderSrc = 
        "#version 330 core                                          \n"
        "layout (location = 0) in vec3 aPos;                        \n"
        "                                                           \n"
        "out vec3 TexCoords;                                        \n"
        "                                                           \n"
        "uniform mat4 uProjMat;                                     \n"
        "uniform mat4 uViewMat;                                     \n"
        "                                                           \n"
        "void main()                                                \n"
        "{                                                          \n"
        "   TexCoords = aPos;                                       \n"
        "   gl_Position = uProjMat * uViewMat * vec4(aPos,1.0);     \n"
        "}                                                          \n";
    const char* fragmentShaderSrc = 
        "#version 330 core                                          \n"
        "out vec4 FragColor;                                        \n"
        "                                                           \n"
        "in vec3 TexCoords;                                         \n"
        "                                                           \n"
        "uniform samplerCube uSkybox;                               \n"
        "                                                           \n"
        "void main()                                                \n"
        "{                                                          \n"
        "   FragColor = texture(uSkybox, TexCoords);                \n"
        "}                                                          \n";
    if (!m_Shader.LoadFromString(vertexShaderSrc,fragmentShaderSrc)) {
        throw std::runtime_error("Sky Node shader init failed");
    }
}
OGLSkyNode::~OGLSkyNode()
{
    if (m_CubeTexId) {
        glDeleteTextures(1, &m_CubeTexId);
        m_CubeTexId = 0;
    }
}

bool OGLSkyNode::VOnRestore(Scene* pScene)
{
    //TODO
    return true;
}

bool OGLSkyNode::VRender(Scene* pScene)
{
    OGLRenderer* rndr = static_cast<OGLRenderer*>(pScene->GetRenderer());
    rndr->DrawVertexBuffer(m_VertBuf);
    return true;
}

bool OGLSkyNode::VPreRender(Scene* pScene)
{
    //const Mat4x4& viewMat = m_Camera->VGet()->GetToWorld();
    OGLRenderer* rndr = static_cast<OGLRenderer*>(pScene->GetRenderer());
    rndr->SetShader(m_Shader);
    //rndr->VSetViewTransform(&viewMat);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTexId);
    return SceneNode::VPreRender(pScene);
}

