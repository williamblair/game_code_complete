#include <GL/glew.h>
#include "OGLAlphaPassRenderState.h"

OGLAlphaPassRenderState::OGLAlphaPassRenderState()
{
    bBlendAlreadyEnabled = (bool)glIsEnabled(GL_BLEND);
    if (!bBlendAlreadyEnabled) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

OGLAlphaPassRenderState::~OGLAlphaPassRenderState()
{
    if (!bBlendAlreadyEnabled) {
        glDisable(GL_BLEND);
    }
}

