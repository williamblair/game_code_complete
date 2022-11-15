#ifndef LIGHT_MANAGER_H_INCLUDED
#define LIGHT_MANAGER_H_INCLUDED

#include <list>

#include "LightNode.h"
#include "ConstantBuffer_Lighting.h"

typedef std::list<std::shared_ptr<LightNode>> Lights;

class LightManager
{
    friend class Scene;
public:

    LightManager();
    ~LightManager();

    int GetLightCount(const SceneNode* pNode) { (void)pNode; return m_Lights.size(); }
    const Vec4* GetLightAmbient(const SceneNode* pNode) { (void)pNode; return &m_vLightAmbient; }
    const Vec4* GetLightDirection(const SceneNode* pNode) { (void)pNode; return m_vLightDir; }
    const Color* GetLightDiffuse(const SceneNode* pNode) { (void)pNode; return m_vLightDiffuse; }

    void CalcLighting(Scene* pScene);
    void CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode* pNode);
protected:
    Lights m_Lights;
    Vec4 m_vLightDir[MAXIMUM_LIGHTS_SUPPORTED];
    Color m_vLightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
    Vec4 m_vLightAmbient;
};

#endif // LIGHT_MANAGER_H_INCLUDED
