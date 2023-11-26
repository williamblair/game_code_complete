#ifndef GCC4_TRANSFORM_COMPONENT_H_INCLUDED
#define GCC4_TRANSFORM_COMPONENT_H_INCLUDED

#include "ActorComponent.h"
#include "GCCMath.h"
#include "xml.h"

// This component implementatino represents the physical aspect of an
// actor. It defines the transform and doesn't register with the
// physics system at all.

class TransformComponent : public ActorComponent
{
public:
    static const char* g_Name;
    virtual const char* VGetName() const { return g_Name; }
    
    // TODO - remove/update ActorComponent class?
    virtual ComponentId VGetComponentId() const { return ComponentId(0xA); /* 0xA random, TODO remove */ }
    virtual std::string VGetPrintInfo() const { return std::string(g_Name); }
    
    TransformComponent() :
        m_Transform(Mat4x4::g_Identity)
    {}
    virtual bool VInit(tinyxml2::XMLElement* pData) override;
    // TODO - update ActorComponent class
    virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument* pDoc)/* override*/;
    
    // transform functions
    Mat4x4 GetTransform() const { return m_Transform; }
    void SetTransform(const Mat4x4& mat) { m_Transform = mat; }
    Vec3 GetPosition() const { return m_Transform.GetPosition(); }
    void SetPosition(const Vec3& pos) { m_Transform.SetPosition(pos); }
    Vec3 GetLookAt() const { return m_Transform.GetDirection(); }

private:
    Mat4x4 m_Transform;
};

#endif // GCC4_TRANSFORM_COMPONENT_H_INCLUDED
