#include "PhysicsComponent.h"

using namespace tinyxml2;

const char* PhysicsComponent::g_Name = "PhysicsComponent";

PhysicsComponent::PhysicsComponent()
{
    //TODO
}

PhysicsComponent::~PhysicsComponent()
{
    //TODO
}

XMLElement* PhysicsComponent::VGenerateXml() override
{
    //TODO
    return nullptr;
}

// ActorComponent interface
bool PhysicsComponent::VInit(XMLElement* pData) override
{
    //TODO
    return true;
}

void PhysicsComponent::VPostInit() override
{
    //TODO
}

void PhysicsComponent::VUpdate(int deltaMs) override
{
    //TODO
}

// Physics functions
void PhysicsComponent::ApplyForce(const Vec3& direction, float forceNewtons)
{
    //TODO
}

void PhysicsComponent::ApplyTorque(const Vec3& direction, float forecNewtons)
{
    //TODO
}

bool PhysicsComponent::KinematicMove(const Mat4x4& transform)
{
    //TODO
    return true;
}

// acceleration
void PhysicsComponent::ApplyAcceleration(float acceleration)
{
    //TODO
}

void PhysicsComponent::RemoveAcceleration()
{
    //TODO
}

void PhysicsComponent::ApplyAngularAcceleration(float acceleration)
{
    //TODO
}

void PhysicsComponent::RemoveAngularAcceleration()
{
    //TODO
}

Vec3 PhysicsComponent::GetVelocity()
{
    //TODO
    return Vec3();
}

void PhysicsComponent::SetVelocity(const Vec3& velocity)
{
    //TODO
}

void PhysicsComponent::RotateY(float angleRadians)
{
    //TODO
}

void PhysicsComponent::SetPosition(float x, float y, float z)
{
    //TODO
}

void PhysicsComponent::Stop()
{
    //TODO
}

