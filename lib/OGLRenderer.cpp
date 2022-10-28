#include "OGLRenderer.h"

OGLRenderer::OGLRenderer() :
    mWindow(nullptr),
    mWidth(0),
    mHeight(0)
{}

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
void OGLRenderer::VSetWorldTransform(const Mat4x4* m)
{
    //TODO
}
void OGLRenderer::VSetViewTransform(const Mat4x4* m)
{
    //TODO
}
void OGLRenderer::VSetProjectionTransform(const Mat4x4* m)
{
    //TODO
}
std::shared_ptr<IRenderState> OGLRenderer::VPrepareAlphaPass()
{
    //TODO
    return std::shared_ptr<IRenderState>();
}
std::shared_ptr<IRenderState> OGLRenderer::VPrepareSkyboxPass()
{
    //TODO
    return std::shared_ptr<IRenderState>();
}
void OGLRenderer::VDrawLine(const Vec3& from, const Vec3& to, const Color& color)
{
    //TODO
}
