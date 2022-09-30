#ifndef SCENENODEPROPERTIES_H_INCLUDED
#define SCENENODEPROPERTIES_H_INCLUDED

#include <string>

#include <ComponentTypes.h>
#include <GCCMath.h>
#include <RenderPass.h>
#include <Material.h>
#include <AlphaType.h>

class SceneNodeProperties
{

friend class SceneNode;
    
public:
    
    const ActorId& GetActorId() const { return m_ActorId; }
    Mat4x4 const& GetToWorld() const { return m_ToWorld; }
    Mat4x4 const& GetFromWorld() const { return m_FromWorld; }
    void Transform(Mat4x4* toWorld, Mat4x4* fromWorld) const;
    
    const char* Name() const { return m_Name.c_str(); }
    
    bool HasAlpha() const { return m_Material.HasAlpha(); }
    virtual float Alpha() const { return m_Material.GetAlpha(); }
    
    RenderPass GetRenderPass() const { return m_RenderPass; }
    float Radius() const { return m_Radius; }
    
    const Material& GetMaterial() const { return m_Material; }

protected:
    ActorId m_ActorId;
    std::string m_Name;
    Mat4x4 m_ToWorld, m_FromWorld;
    float m_Radius;
    RenderPass m_RenderPass;
    Material m_Material;
    AlphaType m_AlphaType;
    
    void SetAlpha(const float alpha) {
        m_AlphaType = AlphaMaterial;
        m_Material.SetAlpha(alpha);
    }
};

#endif // SCENENODEPROPERTIES_H_INCLUDED
