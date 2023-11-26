#ifndef GCC4_OGLALPHAPASS_RENDERSTATE_H_INCLUDED
#define GCC4_OGLALPHAPASS_RENDERSTATE_H_INCLUDED

#include "IRenderState.h"

class OGLAlphaPassRenderState : public IRenderState
{
public:
    OGLAlphaPassRenderState();
    ~OGLAlphaPassRenderState();
    virtual std::string VToString() { return "OGLAlphaPassRenderState"; }
private:
    bool bBlendAlreadyEnabled;
};

#endif // GCC4_OGLALPHAPASS_RENDERSTATE_H_INCLUDED
