#include "BulletPhysics.h"

BulletPhysics::~BulletPhysics()
{
    //TODO
}
    
// Initialization and maintenance of the physics world
bool BulletPhysics::VInitialize()
{
    //TODO
    return true;
}
void BulletPhysics::VSyncVisibleScene()
{
    //TODO
}
void BulletPhysics::VOnUpdate(float deltaSeconds)
{
    //TODO
}

// Initialization of physics objects
void BulletPhysics::VAddSphere(
    float radius,
    WeakActorPtr pGameActor,
    const Mat4x4& initialTransform,
    const std::string& densityStr,
    const std::string& physicsMaterial)
{
    //TODO
}
void BulletPhysics::VAddBox(
    const Vec3& dimensions,
    WeakActorPtr pGameActor,
    const Mat4x4& initialTransform,
    const std::string& densityStr,
    const std::string& physicsMaterial)
{
    //TODO
}

void BulletPhysics::VRemoveActor(ActorId id)
{
    //TODO
}

// Debugging
void BulletPhysics::VRenderDiagnostics()
{
    //TODO
}

// Physics world modifiers
void BulletPhysics::VCreateTrigger(
    WeakActorPtr pActor,
    const Vec3& pos,
    const float dim)
{
    //TODO
}
void BulletPhysics::VApplyForce(const Vec3& dir, float newtons, ActorId id)
{
    //TODO
}
void BulletPhysics::VApplyTorque(const Vec3& dir, float newtons, ActorId id)
{
    //TODO
}
bool BulletPhysics::VKinematicMove(const Mat4x4& mat, ActorId id)
{
    //TODO
    return true;
}


void BulletPhysics::LoadXml()
{
    //TODO
}
float BulletPhysics::LookupSpecificGravity(const std::string& densityStr)
{
    //TODO
    return 0.0f;
}
MaterialData BulletPhysics::LookupMaterialData(const std::string& materialStr)
{
    //TODO
    return MaterialData(0.0f,0.0f);
}

btRigidBody* BulletPhysics::FindBulletRigidBody(ActorId id) const
{
    //TODO
    return nullptr;
}

ActorId BulletPhysics::FindActorId(btRigidBody const*) const
{
    //TODO
    return INVALID_ACTOR_ID;
}

    
// helpers for sending events relating to collision pairs
void BulletPhysics::SendCollisionPairAddEvent(
    btPersistentManifold const* manifold,
    btRigidBody const* body0, btRigidBody const* body1)
{
    //TODO
}
void BulletPhysics::SendCollisionPairRemoveEvent(
    btRigidBody const* body0, btRigidBody const* body1)
{
    //TODO
}

// common functionality used by VAddSphere, VAddBox, etc.
void BulletPhysics::AddShape(
    StrongActorPtr pGameActor,
    btCollisionShape* shape,
    float mass,
    const std::string& physicsMaterial)
{
    //TODO
}

// helper for cleaning up objects
void BulletPhysics::RemoveCollisionObject(btCollisionObject* removeMe)
{
    //TODO
}

// callback from bullet for each physics time step. set in VInitialize
void BulletPhysics::BulletInternalTickCallback(
    btDynamicsWorld* const pWorld,
    btScalar const timeStep)
{
    //TODO
}
