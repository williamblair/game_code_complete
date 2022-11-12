#include "OGLRenderer.h"
#include "OGLAlphaPassRenderState.h"
#include "OGLSkyBoxPassRenderState.h"

OGLRenderer::OGLRenderer() :
    mWindow(nullptr),
    mWidth(0),
    mHeight(0)
{
    m_pCurShader = nullptr;
}

OGLRenderer::~OGLRenderer()
{
    if (mWindow) {
        SDL_GL_DeleteContext(mContext);
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
        SDL_Quit();
    }
}

bool OGLRenderer::Init(int width, int height)
{
    mWidth = width;
    mHeight = height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        printf("OGLRenderer::Init failed: %s\n", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "GCC4 Engine",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        mWidth, mHeight,
        SDL_WINDOW_OPENGL
    );
    if (!mWindow) {
        printf("OGLRenderer::Init failed to create window: %s\n", SDL_GetError());
        return false;
    }

    // set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // create a render context
    mContext = SDL_GL_CreateContext(mWindow);
    
    // enable vsync
    SDL_GL_SetSwapInterval(1);

    // init GLEW
    glewInit();

    // hide/capture mouse cursor
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glFrontFace(GL_CW); // match DirectX winding order
    //glEnable(GL_CULL_FACE); // TODO

    return true;
}

void OGLRenderer::VSetBackgroundColor(uint8_t bgA, uint8_t bgR, uint8_t bgG, uint8_t bgB)
{
    glClearColor(float(bgR)/255.0f, float(bgG)/255.0f, float(bgB)/255.0f, float(bgA)/255.0f);
}

bool OGLRenderer::VOnRestore()
{
    //TODO?
    return true;
}

void OGLRenderer::VShutdown()
{
    if (mWindow) {
        SDL_GL_DeleteContext(mContext);
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
        SDL_Quit();
    }
}

bool OGLRenderer::VPreRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return true;
}

bool OGLRenderer::VPostRender()
{
    //TODO - calc delay time
    SDL_Delay((1.0f/60.0f)*1000.0f);
    SDL_GL_SwapWindow(mWindow);
    return true;
}

bool OGLRenderer::VCalcLighting(Lights* lights, int maximumLights)
{
    //TODO
    return true;
}

void OGLRenderer::VSetWorldTransform(const Mat4x4* m) {
    mModelMat = *m;
    if (m_pCurShader) {
        m_pCurShader->SetMat4("uModelMat", mModelMat);
    }
}
void OGLRenderer::VSetViewTransform(const Mat4x4* m) {
    mViewMat = *m;
    if (m_pCurShader) {
        m_pCurShader->SetMat4("uViewMat", mViewMat);
    }
}
void OGLRenderer::VSetProjectionTransform(const Mat4x4* m) {
    mProjMat = *m;
    if (m_pCurShader) {
        m_pCurShader->SetMat4("uProjMat", mProjMat);
    }
}

std::shared_ptr<IRenderState> OGLRenderer::VPrepareAlphaPass() { return std::make_shared<OGLAlphaPassRenderState>(); }
std::shared_ptr<IRenderState> OGLRenderer::VPrepareSkyboxPass() { return std::make_shared<OGLSkyBoxPassRenderState>(); }

void OGLRenderer::VDrawLine(const Vec3& from, const Vec3& to, const Color& color)
{
    //TODO
}

void OGLRenderer::SetShader(OGLShader& shdr)
{
    if (&shdr == m_pCurShader) {
        return;
    }
    Mat4x4 mvpMat = mProjMat * mViewMat * mModelMat;

    // Bind the shader
    shdr.Use();

    // Set uniforms
    shdr.SetMat4("uMvpMatrix", mvpMat);
    shdr.SetMat4("uModelMat", mModelMat);
    shdr.SetMat4("uViewMat", mViewMat);
    shdr.SetMat4("uProjMat", mProjMat);

    m_pCurShader = &shdr;
}
void OGLRenderer::DrawVertexBuffer(const OGLVertexBuffer& vb)
{
    glBindVertexArray(vb.mVAO);
    if (vb.mNumIndices > 0) {
        // TODO - configurable drawing mode
        glDrawElements(GL_TRIANGLES, vb.mNumIndices, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vb.mNumVerts);
    }
}


