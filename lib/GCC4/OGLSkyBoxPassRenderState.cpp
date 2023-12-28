#include "OGLSkyBoxPassRenderState.h"
#include <GL/glew.h>

OGLSkyBoxPassRenderState::OGLSkyBoxPassRenderState()
{
    GLboolean bDepthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &bDepthMask);
    bDepthMaskWasEnabled = (bool)bDepthMask;
    if (bDepthMaskWasEnabled) {
        glDepthMask(GL_FALSE);
    }
}

OGLSkyBoxPassRenderState::~OGLSkyBoxPassRenderState()
{
    if (bDepthMaskWasEnabled) {
        glDepthMask(GL_TRUE);
    }
}

