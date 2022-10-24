#include <cassert>

#include "LightManager.h"

void LightManager::CalcLighting(Scene* pScene)
{
    //TODO
    //pScene->GetRenderer()->VCalcLighting(&m_Lights, MAXIMUM_LIGHTS_SUPPORTED);

    int count = 0;
    assert(m_Lights.size() < MAXIMUM_LIGHTS_SUPPORTED);
    for (Lights::iterator i = m_Lights.begin(); i != m_Lights.end(); ++i, ++count)
    {
        std::shared_ptr<LightNode> light = *i;
        if (count == 0)
        {
            // Light 0 is the only one we use for ambient lighting. The rest are
            // ignored in the simple shaders used for GameCode4.
            Color ambient = light->VGet()->GetMaterial().GetAmbient();
            m_vLightAmbient = Vec4(ambient.r, ambient.g, ambient.b, 1.0f);
        }

        Vec3 lightDir = light->GetDirection();
        m_vLightDir[count] = Vec4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
        m_vLightDiffuse[count] = light->VGet()->GetMaterial().GetDiffuse();
    }
}

void LightManager::CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode* pNode)
{
    // TODO
#if 0
    int count = GetLightCount(pNode);
    if (count)
    {
        pLighting->m_vLightAmbient = *GetLightAmbient(pNode);
        memcpy(pLighting->m_vLightDir, GetLightDirection(pNode), sizeof(Vec4)*count);
        memcpy(pLighting->m_vLightDiffuse, GetLightDiffuse(pNode), sizeof(Vec4)*count);
        pLighting->m_nNumLights = count;
    }
#endif
}

