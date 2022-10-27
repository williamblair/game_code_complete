#ifndef GCC4_OGL_RENDERER_H_INCLUDED
#define GCC4_OGL_RENDERER_H_INCLUDED

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "IRenderer.h"
#include "GCCMath.h"

class OGLRenderer : public IRenderer
{
public:
    OGLRenderer();
    virtual ~OGLRenderer();

    bool Init(int width, int height);

    virtual void VSetBackgroundColor(uint8_t bgA, uint8_t bgR, uint8_t bgG, uint8_t bgB);
    virtual bool VOnRestore();
    virtual void VShutdown();
    virtual bool VPreRender();
    virtual bool VPostRender();
    virtual bool VCalcLighting(Lights* lights, int maximumLights);
    virtual void VSetWorldTransform(const Mat4x4* m);
    virtual void VSetViewTransform(const Mat4x4* m);
    virtual void VSetProjectionTransform(const Mat4x4* m);
    virtual std::shared_ptr<IRenderState> VPrepareAlphaPass();
    virtual std::shared_ptr<IRenderState> VPrepareSkyboxPass();
    virtual void VDrawLine(const Vec3& from, const Vec3& to, const Color& color);
private:
    SDL_Window* mWindow;
    SDL_GLContext mContext;
    int mWidth, mHeight;

    Mat4x4 mModelMat; // model space to world space matrix
    Mat4x4 mViewMat; // world space to view/camera space matrix
    Mat4x4 mProjMat; // projection matrix
};

#endif // GCC4_OGL_RENDERER_H_INCLUDED
