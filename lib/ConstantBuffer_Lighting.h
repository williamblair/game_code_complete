#ifndef GCC4_CONSTANT_BUFFER_LIGHTING_H_INCLUDED
#define GCC4_CONSTANT_BUFFER_LIGHTING_H_INCLUDED

#include <GCCMath.h>

#define MAXIMUM_LIGHTS_SUPPORTED 8

struct ConstantBuffer_Lighting
{
    Vec3 m_vLightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
    Vec3 m_vLightDir[MAXIMUM_LIGHTS_SUPPORTED];
    Vec3 m_vLightAmbient;
    int m_NumLights;
};

#endif // CONSTANT_BUFFER_LIGHTING_H_INCLUDED
