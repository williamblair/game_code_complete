#ifndef IRENDERER_H_INCLUDED
#define IRENDERER_H_INCLUDED

#include <cstdint>
#include <memory>
#include <string>

#include "LightManager.h"
#include "GCCMath.h"
#include "IRenderState.h"

class IRenderer
{
public:
    virtual void VSetBackgroundColor(uint8_t bgA, uint8_t bgR, uint8_t bgG, uint8_t bgB) = 0;
    virtual bool VOnRestore() = 0;
    virtual void VShutdown() = 0;
    virtual bool VPreRender() = 0;
    virtual bool VPostRender() = 0;
    virtual bool VCalcLighting(Lights* lights, int maximumLights) = 0;
    virtual void VSetWorldTransform(const Mat4x4* m) = 0;
    virtual void VSetViewTransform(const Mat4x4* m) = 0;
    virtual void VSetProjectionTransform(const Mat4x4* m) = 0;
    virtual std::shared_ptr<IRenderState> VPrepareAlphaPass() = 0;
    virtual std::shared_ptr<IRenderState> VPrepareSkyboxPass() = 0;
    virtual void VDrawLine(const Vec3& from, const Vec3& to, const Color& color) = 0;
    virtual int VGetWidth() = 0;
    virtual int VGetHeight() = 0;
};

#endif // IRENDERER_H_INCLUDED
