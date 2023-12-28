#ifndef GCC4_PHYSICS_COMPONENT_H_INCLUDED
#define GCC4_PHYSICS_COMPONENT_H_INCLUDED

#include "ActorComponent.h"
#include "IGamePhysics.h"
#include "xml.h"

class PhysicsComponent : public ActorComponent
{
public:
    static const char* g_Name;
    virtual const char* VGetName() const override { return g_Name; }

    // TODO - remove/update ActorComponent class?
    //virtual ComponentId VGetComponentId() const { return ComponentId(0xB); /* 0xB random, TODO remove */ }
    //virtual std::string VGetPrintInfo() const { return std::string(g_Name); }

    PhysicsComponent();
    virtual ~PhysicsComponent();
    virtual tinyxml2::XMLElement* VGenerateXml() override;

    // ActorComponent interface
    virtual bool VInit(tinyxml2::XMLElement* pData) override;
    virtual void VPostInit() override;
    virtual void VUpdate(int deltaMs) override;

    // Physics functions
    void ApplyForce(const Vec3& direction, float forceNewtons);
    void ApplyTorque(const Vec3& direction, float forecNewtons);
    bool KinematicMove(const Mat4x4& transform);

    // acceleration
    void ApplyAcceleration(float acceleration);
    void RemoveAcceleration();
    void ApplyAngularAcceleration(float acceleration);
    void RemoveAngularAcceleration();

    Vec3 GetVelocity();
    void SetVelocity(const Vec3& velocity);
    void RotateY(float angleRadians);
    void SetPosition(float x, float y, float z);
    void Stop();

protected:
    void CreateShape();
    void BuildRigidBodyTransform(tinyxml2::XMLElement* pTransformElement);
    
    float m_acceleration, m_angularAcceleration;
    float m_maxVelocity, m_maxAngularVelocity;

    std::string m_shape;
    std::string m_density;
    std::string m_material;

    Vec3 m_RigidBodyLocation; // offset from actor position
    Vec3 m_RigidBodyOrientation; // offset from actor orientation
    Vec3 m_RigidBodyScale; // offset from actor scale
    
    std::shared_ptr<IGamePhysics> m_pGamePhysics;
};

#endif // GCC4_PHYSICS_COMPONENT_H_INCLUDED

