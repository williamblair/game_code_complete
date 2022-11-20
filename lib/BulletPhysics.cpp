#include <cassert>
#include <iterator>
#include <algorithm>
#include <memory>
#include "ComponentTypes.h"
#include "BulletPhysics.h"
#include "BulletDebugDrawer.h"
#include "ActorMotionState.h"
#include "Vec3ToBtVector3.h"
#include "MakeStrongPtr.h"
#include "Actor.h"
#include "EventManager.h"

BulletPhysics::~BulletPhysics()
{
    // delete any physics objects still in the world
    // iterate backwards because removing the last object
    // doesn't affect the other objects
    for (int i=m_pDynamicsWorld->getNumCollisionObjects()-1; i>=0; --i) {
        btCollisionObject* const obj =
            m_pDynamicsWorld->getCollisionObjectArray()[i];
        RemoveCollisionObject(obj);
    }
    m_actorIdToRigidBody.clear();
    m_rigidBodyToActorId.clear();
    if (m_pCollisionConfiguration) { delete m_pCollisionConfiguration; }
    if (m_pDispatcher) { delete m_pDispatcher; }
    if (m_pBroadPhase) { delete m_pBroadPhase; }
    if (m_pSolver) { delete m_pSolver; }
    if (m_pDynamicsWorld) { delete m_pDynamicsWorld; }
    if (m_pDebugDrawer) { delete m_pDebugDrawer; }
}
    
// Initialization and maintenance of the physics world
bool BulletPhysics::VInitialize()
{
    LoadXml();
    
    // this controls how Bullet does internal memory management
    m_pCollisionConfiguration =
        new btDefaultCollisionConfiguration();
    
    // Manages how bullet detects precise collisions between objects
    m_pDispatcher =
        new btCollisionDispatcher(m_pCollisionConfiguration);
    
    // Bullet uses this to quickly/imprecisely detect collisions
    // between objects. Once a possible collision passes the broad
    // phase, it will be passed to the slower but more precise narrow
    // phase collision detection (btCollisionDispatcher)
    m_pBroadPhase = new btDbvtBroadphase();
    
    // Manages constraints which apply forces to the physics sim.
    // Used for e.g. springs, motors. We don't use any right now.
    m_pSolver = new btSequentialImpulseConstraintSolver();
    
    // This is the main Bullet interface point. Pass in all these
    // components to customize its behavior.
    m_pDynamicsWorld = new btDiscreteDynamicsWorld(
        m_pDispatcher,
        m_pBroadPhase,
        m_pSolver,
        m_pCollisionConfiguration
    );
    
    // Also setup functionality for debug drawing
    m_pDebugDrawer = new BulletDebugDrawer();
    
    if (!m_pCollisionConfiguration ||
        !m_pDispatcher ||
        !m_pBroadPhase ||
        !m_pSolver ||
        !m_pDynamicsWorld ||
        !m_pDebugDrawer)
    {
        printf("Failed to init bullet physics\n");
        return false;
    }
    
    m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer);
    
    // and set the internal tick callback to our own method
    m_pDynamicsWorld->setInternalTickCallback(BulletInternalTickCallback);
    m_pDynamicsWorld->setWorldUserInfo(this);
    
    return true;
}
void BulletPhysics::VSyncVisibleScene()
{
    // check all the existing actor's bodies for changes.
    // if there is a change, send the appropriate event for the game system.
    for (ActorIDToBulletRigidBodyMap::const_iterator it =
            m_actorIdToRigidBody.begin();
        it != m_actorIdToRigidBody.end();
        ++it)
    {
        ActorId id = it->first;
        
        // Get the ActorMotionState. This object is updated by Bullet.
        // It's safe to cast the btMotionState to ActorMotionState,
        // because all the bodies in m_actorIdToRigidBody were created
        // through AddShape()
        ActorMotionState* const actorMotionState =
            static_cast<ActorMotionState*>(it->second->getMotionState());
        assert(actorMotionState);
        
        StrongActorPtr pGameActor =
            MakeStrongPtr(g_pApp->m_pGame->VGetActor(id));
        if (!pGameActor || !actorMotionState) {
            continue;
        }
        std::shared_ptr<TransformComponent> pTransformComp =
            MakeStrongPtr(
                pGameActor->GetComponent<TransformComponent>(
                    TransformComponent::g_Name
                )
            );
        if (!pTransformComp) { continue; }
        if (pTransformComp->GetTransform() !=
            actorMotionState->m_worldToPositionTransform)
        {
            // Bullet has moved the actor's physics object
            // Sync the transform and inform the game an actor has moved.
            pTransformComp->SetTransform(
                actorMotionState->m_worldToPositionTransform
            );
            IEventManager::GetInstance()->VQueueEvent(
                new EvtData_Move_Actor(
                    id, actorMotionState->m_worldToPositionTransform
                )
            );
        }
    }
}
void BulletPhysics::VOnUpdate(float deltaSeconds)
{
    // Bullet uses an internal fixed timestep (default 1/60th sec)
    // we pass in 4 as a max number of steps. Bullet will run the
    // simulation in increments of teh fixed timestep until "deltaSeconds"
    // amount of time has passed, but only a max of 4
    m_pDynamicsWorld->stepSimulation(deltaSeconds, 4);
}

// Initialization of physics objects
void BulletPhysics::VAddSphere(
    float radius,
    WeakActorPtr pGameActor,
    //const Mat4x4& initialTransform,
    const std::string& densityStr,
    const std::string& physicsMaterial)
{
    StrongActorPtr pStrongActor = MakeStrongPtr(pGameActor);
    if (!pStrongActor) { return; }
    
    // create the collision body, which specifies the shape of the object
    btSphereShape* const collisionShape = new btSphereShape(radius);
    
    // calculate absolute mass from specificGravity
    float specificGravity = LookupSpecificGravity(densityStr);
    const float volume = (4.0f/3.0f)*M_PI*radius*radius*radius;
    const btScalar mass = volume*specificGravity;
    
    AddShape(pStrongActor, collisionShape, mass, physicsMaterial);
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
void BulletPhysics::VRenderDiagnostics() {
    m_pDynamicsWorld->debugDrawWorld();
}

// Physics world modifiers
void BulletPhysics::VCreateTrigger(
    WeakActorPtr pActor,
    const Vec3& pos,
    const float dim)
{
    StrongActorPtr pStrongActor = MakeStrongPtr(pActor);
    if (!pStrongActor) {
        return;
    }
    
    // create the collision body, which specifies the shape of the object
    btBoxShape* const boxShape = new btBoxShape(
        Vec3ToBtVector3(Vec3(dim,dim,dim))
    );
    
    // triggers are immovable. 0 mass signals this to Bullet.
    btScalar const mass = 0;
    
    // set the initial position of the body from the actor
    Mat4x4 triggerTrans = Mat4x4::g_Identity;
    triggerTrans.SetPosition(pos);
    ActorMotionState* const pMyMotionState =
        new ActorMotionState(triggerTrans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(
        mass,
        pMyMotionState,
        boxShape,
        btVector3(0,0,0)
    );
    btRigidBody* const pBody = new btRigidBody(rbInfo);
    m_pDynamicsWorld->addRigidBody(pBody);
    
    // a trigger is just a box that doesn't collide with anything.
    // that's what "CF_NO_CONTACT_RESPONSE" indicates
    pBody->setCollisionFlags(
        pBody->getCollisionFlags() | btRigidBody::CF_NO_CONTACT_RESPONSE
    );
    
    m_actorIdToRigidBody[pStrongActor->GetId()] = pBody;
    m_rigidBodyToActorId[pBody] = pStrongActor->GetId();
}
void BulletPhysics::VApplyForce(const Vec3& dir, float newtons, ActorId id)
{
    btRigidBody* pRigidBody = FindBulletRigidBody(id);
    if (!pRigidBody) {
        printf("VApplyForce failed to find rigid body\n");
        return;
    }
    
    btVector3 const force(dir.x*newtons, dir.y*newtons, dir.z*newtons);
    pRigidBody->applyCentralImpulse(force);    
}
void BulletPhysics::VApplyTorque(const Vec3& dir, float newtons, ActorId id)
{
    btRigidBody* pRigidBody = FindBulletRigidBody(id);
    if (!pRigidBody) {
        printf("VApplyTorque failed to find rigid body\n");
        return;
    }
    btVector3 const torque(dir.x*newtons,dir.y*newtons,dir.z*newtons);
    pRigidBody->applyTorqueImpulse(torque);
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
    assert(pGameActor);
    
    ActorId actorId = pGameActor->GetId();
    assert(
        (m_actorIdToRigidBody.find(actorId) == m_actorIdToRigidBody.end()) && 
        "Actor with more than one phsyics body?"
    );
    
    // lookup material
    MaterialData material(LookupMaterialData(physicsMaterial));
    
    // localInertia defines how the object's mass is distributed
    btVector3 localInertia(0.0f,0.0f,0.0f);
    if (mass > 0.0f) {
        shape->calculateLocalInertia(mass,localInertia);
    }
    
    Mat4x4 transform = Mat4x4::g_Identity;
    std::shared_ptr<TransformComponent> pTransformComp =
        MakeStrongPtr(
            pGameActor->GetComponent<TransformComponent>(
                TransformComponent::g_Name
            )
        );
    if (!pTransformComp) {
        // physics can;t work on an actor that doesn't have a transform component
        printf("No transform component in actor w/ id %llu\n", uint64_t(actorId));
        return;
    }
    transform = pTransformComp->GetTransform();
    
    // set the initial transform of the body from the actor
    ActorMotionState* const pMyMotionState =
        new ActorMotionState(transform);
    
    btRigidBody::btRigidBodyConstructionInfo rbInfo(
        mass,
        pMyMotionState,
        shape,
        localInertia
    );
    
    // setup the material properties
    rbInfo.m_restitution = material.restitution;
    rbInfo.m_friction = material.friction;
    
    btRigidBody* const body = new btRigidBody(rbInfo);
    m_pDynamicsWorld->addRigidBody(body);
    
    // add it to the collection to be checked for changes in VSyncVisibleScene
    m_actorIdToRigidBody[actorId] = body;
    m_rigidBodyToActorId[body] = actorId;
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
    assert(pWorld);
    assert(pWorld->getWorldUserInfo());
    BulletPhysics* const bulletPhysics =
        static_cast<BulletPhysics*>(pWorld->getWorldUserInfo());
    
    CollisionPairs currentTickCollisionPairs;
    
    // look at all existing contacts
    btDispatcher* const pDispatcher = pWorld->getDispatcher();
    for (int manifoldIdx = 0;
        manifoldIdx < pDispatcher->getNumManifolds();
        ++manifoldIdx)
    {
        // get the "manifold", or data corresponding to a contact point
        // between two physics objects
        btPersistentManifold const* const pManifold =
            pDispatcher->getManifoldByIndexInternal(manifoldIdx);
        if (!pManifold) { continue; }
        
        // get the two bodies used in the manifold. Bullet stores them
        // as void* so we must cast them back to btRigidBodys. We know
        // this is safe because we only ever add btRigidBodys to the
        // simulation
        //btRigidBody const* const pBody0 =
        //    static_cast<btRigidBody* const>(pManifold->getBody0());
        //btRigidBody const* const pBody1 =
        //    static_cast<btRigidBody* const>(pManifold->getBody1());
        btRigidBody const* const pBody0 = (btRigidBody*)pManifold->getBody0();
        btRigidBody const* const pBody1 = (btRigidBody*)pManifold->getBody1();
        
        // always create the pair in a predictable order
        const bool swapped = pBody0 > pBody1;
        btRigidBody const* const pSortedBodyA = swapped ? pBody1 : pBody0;
        btRigidBody const* const pSortedBodyB = swapped ? pBody0 : pBody1;
        
        CollisionPair const thisPair =
            std::make_pair(pSortedBodyA,pSortedBodyB);
        currentTickCollisionPairs.insert(thisPair);
        
        if (bulletPhysics->m_previousTickCollisionPairs.find(thisPair) ==
            bulletPhysics->m_previousTickCollisionPairs.end())
        {
            // this is a new contact which wasn't in our list before;
            // send an event to the game.
            bulletPhysics->SendCollisionPairAddEvent(pManifold,pBody0,pBody1);
        }
    }
    
    CollisionPairs removedCollisionPairs;
    
    // Use the STL set difference function to find collision pairs that
    // existed during the previous tick but not anymore
    std::set_difference(
        bulletPhysics->m_previousTickCollisionPairs.begin(),
        bulletPhysics->m_previousTickCollisionPairs.end(),
        currentTickCollisionPairs.begin(),
        currentTickCollisionPairs.end(),
        std::inserter(removedCollisionPairs, removedCollisionPairs.begin())
    );
    
    for (CollisionPairs::const_iterator it = removedCollisionPairs.begin(),
            end = removedCollisionPairs.end();
        it != end;
        ++it)
    {
        btRigidBody const* const pBody0 = it->first;
        btRigidBody const* const pBody1 = it->second;
        bulletPhysics->SendCollisionPairRemoveEvent(pBody0,pBody1);
    }
    
    // Save the collision pairs for the next tick
    bulletPhysics->m_previousTickCollisionPairs = currentTickCollisionPairs;
}
