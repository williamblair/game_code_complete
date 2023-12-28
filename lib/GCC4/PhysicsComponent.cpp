#include <GCC4/Actor.h>
#include "GameCodeApp.h"
#include "TransformComponent.h"
#include "MakeStrongPtr.h"
#include "PhysicsComponent.h"

#define DEFAULT_MAX_VELOCITY (7.5f)
#define DEFAULT_MAX_ANGULAR_VELOCITY (1.2f)

using namespace tinyxml2;

const char* PhysicsComponent::g_Name = "PhysicsComponent";

PhysicsComponent::PhysicsComponent()
{
    m_RigidBodyLocation = Vec3(0,0,0);
    m_RigidBodyOrientation = Vec3(0,0,0);
    m_RigidBodyScale = Vec3(1,1,1);

    m_acceleration = 0;
    m_angularAcceleration = 0;
    m_maxVelocity = DEFAULT_MAX_VELOCITY;
    m_maxAngularVelocity = DEFAULT_MAX_ANGULAR_VELOCITY;
}

PhysicsComponent::~PhysicsComponent()
{
    if (m_pGamePhysics) {
        m_pGamePhysics->VRemoveActor(m_pOwner->GetId());
    }
}

XMLElement* PhysicsComponent::VGenerateXml()
{
    XMLDocument* pDoc = new XMLDocument();
    XMLElement* pBaseElement = pDoc->NewElement(VGetName());

    // shape
    //XMLElement* pShape = new XMLElement("Shape");
    //XMLText* pShapeText = new XMLText(m_shape.c_str());
    XMLElement* pShape = pBaseElement->InsertNewChildElement("Shape");
    XMLText* pShapeText = pShape->InsertNewText(m_shape.c_str());
    //pShape->LinkEndChild(pShapeText);
    //pBaseElement->LinkEndChild(pShape);

    // density
    //XMLElement* pDensity = new XMLElement("Density");
    //XMLText* pDensityText = new XMLText(m_density.c_str());
    //pDensity->LinkEndChild(pDensityText);
    //pBaseElement->LinkEndChild(pDensity);
    XMLElement* pDensity = pBaseElement->InsertNewChildElement("Density");
    XMLText* pDensityText = pDensity->InsertNewText(m_density.c_str());

    // material
    //XMLElement* pMaterial = new XMLElement("Material");
    //XMLText* pMaterialText = new XMLText(m_material.c_str());
    //pMaterial->LinkEndChild(pMaterialText);
    //pBaseElement->LinkEndChild(pMaterial);
    XMLElement* pMaterial = pBaseElement->InsertNewChildElement("Material");
    XMLText* pMaterialText = pMaterial->InsertNewText(m_material.c_str());

    // rigid body transform
    //XMLElement* pInitialTransform = new XMLElement("RigidBodyTransform");
    XMLElement* pInitialTransform = pBaseElement->InsertNewChildElement("RigidBodyTransform");

    // initial transform -> position
    //XMLElement* pPosition = new XMLElement("Position");
    XMLElement* pPosition = pInitialTransform->InsertNewChildElement("Position");
    pPosition->SetAttribute("x", std::to_string(m_RigidBodyLocation.x).c_str());
    pPosition->SetAttribute("y", std::to_string(m_RigidBodyLocation.y).c_str());
    pPosition->SetAttribute("z", std::to_string(m_RigidBodyLocation.z).c_str());
    //pInitialTransform->LinkEndChild(pPosition);

    // initial transform -> orientation
    //XMLElement* pOrientation = new XMLElement("Orientation");
    XMLElement* pOrientation = pInitialTransform->InsertNewChildElement("Orientation");
    pOrientation->SetAttribute("yaw", std::to_string(m_RigidBodyOrientation.x).c_str());
    pOrientation->SetAttribute("pitch", std::to_string(m_RigidBodyOrientation.y).c_str());
    pOrientation->SetAttribute("roll", std::to_string(m_RigidBodyOrientation.z).c_str());
    //pInitialTransform->LinkEndChild(pOrientation);

    // initial transform -> scale
    //XMLElement* pScale = new XMLElement("Scale");
    XMLElement* pScale = pInitialTransform->InsertNewChildElement("Scale");
    pScale->SetAttribute("x", std::to_string(m_RigidBodyScale.x).c_str());
    pScale->SetAttribute("y", std::to_string(m_RigidBodyScale.y).c_str());
    pScale->SetAttribute("z", std::to_string(m_RigidBodyScale.z).c_str());
    //pInitialTransform->LinkEndChild(pScale);

    //pBaseElement->LinkEndChild(pInitialTransform);

    //return pBaseElement;
    return pDoc->ToElement();
}

// ActorComponent interface
bool PhysicsComponent::VInit(XMLElement* pData)
{
    m_pGamePhysics = g_pApp->m_pGame->VGetGamePhysics();
    if (!m_pGamePhysics) {
        return false;
    }

    // shape
    XMLElement* pShape = pData->FirstChildElement("Shape");
    if (pShape) {
        m_shape = pShape->FirstChild()->Value();
    }

    // density
    XMLElement* pDensity = pData->FirstChildElement("Density");
    if (pDensity) {
        m_density = pDensity->FirstChild()->Value();
    }

    // material
    XMLElement* pMaterial = pData->FirstChildElement("PhysicsMaterial");
    if (pMaterial) {
        m_material = pMaterial->FirstChild()->Value();
    }

    // initial transform
    XMLElement* pRigidBodyTransform = pData->FirstChildElement("RigidBodyTransform");
    if (pRigidBodyTransform) {
        BuildRigidBodyTransform(pRigidBodyTransform);
    }

    return true;
}

void PhysicsComponent::VPostInit()
{
    if (!m_pOwner) {
        return;
    }

    if (m_shape == "Sphere") {
        m_pGamePhysics->VAddSphere(m_RigidBodyScale.x, m_pOwner, m_density, m_material);
    }
    else if (m_shape == "Box") {
        m_pGamePhysics->VAddBox(m_RigidBodyScale, m_pOwner, m_density, m_material);
    }
    else if (m_shape == "PointCloud") {
        GCC_ERROR("Point Cloud Not Supported!");
    }
}

void PhysicsComponent::VUpdate(int deltaMs)
{
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(
        m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name)
    );
    if (!pTransformComponent) {
        GCC_ERROR("No transform component!");
        return;
    }

    // get the direction the object is facing
    Mat4x4 transform = pTransformComponent->GetTransform();

    if (m_acceleration != 0) {
        // calculate acceleration this frame
        float accelToApplyThisFrame = m_acceleration / 1000.0f * float(deltaMs);

        // Get the current velocity vector and convert to a scalar.
        // The velocity vector is a combination of the direction this actor
        // is going in and the speed of the actor.
        // The scalar is just he speed component.
        Vec3 velocity(m_pGamePhysics->VGetVelocity(m_pOwner->GetId()));
        float velocityScalar = Mag(velocity);

        Vec3 direction(transform.GetDirection());
        m_pGamePhysics->VApplyForce(direction, accelToApplyThisFrame, m_pOwner->GetId());
    }

    if (m_angularAcceleration != 0) {
        float angularAccelToApplyThisFrame = m_angularAcceleration / 1000.0f * float(deltaMs);
        m_pGamePhysics->VApplyTorque(transform.GetUp(), angularAccelToApplyThisFrame, m_pOwner->GetId());
    }
}

// Physics functions
void PhysicsComponent::ApplyForce(const Vec3& direction, float forceNewtons)
{
    m_pGamePhysics->VApplyForce(direction, forceNewtons, m_pOwner->GetId());
}

void PhysicsComponent::ApplyTorque(const Vec3& direction, float forceNewtons)
{
    m_pGamePhysics->VApplyTorque(direction, forceNewtons, m_pOwner->GetId());
}

bool PhysicsComponent::KinematicMove(const Mat4x4& transform)
{
    return m_pGamePhysics->VKinematicMove(transform, m_pOwner->GetId());
}

// acceleration
void PhysicsComponent::ApplyAcceleration(float acceleration)
{
    m_acceleration = acceleration;
}

void PhysicsComponent::RemoveAcceleration()
{
    m_acceleration = 0;
}

void PhysicsComponent::ApplyAngularAcceleration(float acceleration)
{
    m_angularAcceleration = acceleration;
}

void PhysicsComponent::RemoveAngularAcceleration()
{
    m_angularAcceleration = 0;
}

Vec3 PhysicsComponent::GetVelocity()
{
    return m_pGamePhysics->VGetVelocity(m_pOwner->GetId());
}

void PhysicsComponent::SetVelocity(const Vec3& velocity)
{
    m_pGamePhysics->VSetVelocity(m_pOwner->GetId(), velocity);
}

void PhysicsComponent::RotateY(float angleRadians)
{
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(
        m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name)
    );
    if (pTransformComponent) {
        Mat4x4 transform = pTransformComponent->GetTransform();
        Vec3 position = transform.GetPosition();

        //Mat4x4 rotateY;
        //rotateY.BuildRotationY(angleRadians);
        Mat4x4 rotateY = Rotate(angleRadians, Vec3(0,1,0));
        rotateY.SetPosition(position);

        KinematicMove(rotateY);
    } else {
        GCC_ERROR("Attempting to call RotateY() on actor with no transform component!");
    }
}

void PhysicsComponent::SetPosition(float x, float y, float z)
{
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(
        m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name)
    );
    if (pTransformComponent) {
        Mat4x4 transform = pTransformComponent->GetTransform();
        Vec3 position(x,y,z);
        transform.SetPosition(position);
        KinematicMove(transform);
    } else {
        GCC_ERROR("Attempting to call SetPosition() on actor with no transform component!");
    }
}

void PhysicsComponent::Stop()
{
    m_pGamePhysics->VStopActor(m_pOwner->GetId());
}


//void PhysicsComponent::CreateShape()
//{
//    //TODO
//}

void PhysicsComponent::BuildRigidBodyTransform(XMLElement* pTransformElement)
{
    assert(pTransformElement);

    XMLElement* pPositionElement = pTransformElement->FirstChildElement("Position");
    if (pPositionElement) {
        double x = 0, y = 0, z = 0;
        x = pPositionElement->DoubleAttribute("x");
        y = pPositionElement->DoubleAttribute("y");
        z = pPositionElement->DoubleAttribute("z");
        m_RigidBodyLocation = Vec3(float(x), float(y), float(z));
    }

    XMLElement* pOrientationElement = pTransformElement->FirstChildElement("Orientation");
    if (pOrientationElement) {
        double yaw = 0, pitch = 0, roll = 0;
        yaw = pPositionElement->DoubleAttribute("yaw");
        pitch = pPositionElement->DoubleAttribute("pitch");
        roll = pPositionElement->DoubleAttribute("roll");
        m_RigidBodyOrientation = Vec3(float(Deg2Rad(yaw)), float(Deg2Rad(pitch)), float(Deg2Rad(roll)));
    }

    XMLElement* pScaleElement = pTransformElement->FirstChildElement("Scale");
    if (pScaleElement) {
        double x = 0, y = 0, z = 0;
        x = pScaleElement->DoubleAttribute("x");
        y = pScaleElement->DoubleAttribute("y");
        z = pScaleElement->DoubleAttribute("z");
        m_RigidBodyScale = Vec3(float(x), float(y), float(z));
    }
}

