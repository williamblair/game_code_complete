#ifndef OGL_SKYBOXPASS_RENDERSTATE_H_INCLUDED
#define OGL_SKYBOXPASS_RENDERSTATE_H_INCLUDED

#include "IRenderer.h"

class OGLSkyBoxPassRenderState : public IRenderState
{
public:
    OGLSkyBoxPassRenderState();
    ~OGLSkyBoxPassRenderState();
    virtual std::string VToString() { return "OGLSkyBoxPassRenderState"; }
private:
    bool bDepthMaskWasEnabled;
};

#endif // OGL_SKYBOXPASS_RENDERSTATE_H_INCLUDED
