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
#include "EventFactory.h"
#include "GameCodeApp.h"

using namespace tinyxml2;

BulletPhysics::BulletPhysics()
{
    REGISTER_EVENT(EvtDataPhysTriggerEnter);
    REGISTER_EVENT(EvtDataPhysTriggerLeave);
    REGISTER_EVENT(EvtDataPhysCollision);
    REGISTER_EVENT(EvtDataPhysSeparation);
}

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
            std::shared_ptr<EvtDataMoveActor> pMoveEvent(
                new EvtDataMoveActor(
                    id, actorMotionState->m_worldToPositionTransform
                )
            );
            IEventManager::GetInstance()->VQueueEvent(pMoveEvent);
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
    /*const Mat4x4& initialTransform,*/
    const std::string& densityStr,
    const std::string& physicsMaterial)
{
    StrongActorPtr pStrongActor = MakeStrongPtr(pGameActor);
    if (!pStrongActor) {
        // TODO - error log
        return;
    }

    // create the collision body, which specifies the shape of the object
    btBoxShape* const boxShape = new btBoxShape(Vec3ToBtVector3(dimensions));

    // calculate abosulte mass from specificGravity
    float specificGravity = LookupSpecificGravity(densityStr);
    float const volume = dimensions.x * dimensions.y * dimensions.z;
    btScalar mass = volume * specificGravity;

    AddShape(pStrongActor, boxShape, mass, physicsMaterial);
}

void BulletPhysics::VRemoveActor(ActorId id)
{
    btRigidBody* const body = FindBulletRigidBody(id);
    if (body)
    {
        // destroy the body and all its components
        RemoveCollisionObject(body);
        m_actorIdToRigidBody.erase(id);
        m_rigidBodyToActorId.erase(body);
    }
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
    btRigidBody* const body = FindBulletRigidBody(id);
    if (body)
    {
        // warp the body to the new position
        body->setActivationState(DISABLE_DEACTIVATION);
        body->setWorldTransform(Mat4x4ToBtTransform(mat));

        return true;
    }
    return false;
}


void BulletPhysics::LoadXml()
{
    // Load the physics config file and grab the root XML node
    XMLElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement("Config\\physics.xml");
    if (!pRoot) {
        throw std::runtime_error("BulletPhysics LoadXml failed to load Physics.xml");
    }

    // Load all materials
    XMLElement* pParentNode = pRoot->FirstChildElement("PhysicsMaterials");
    if (!pParentNode) {
        throw std::runtime_error("BulletPhysics LoadXml failed to find PhysicsMaterials");
    }
    for (XMLElement* pNode = pParentNode->FirstChildElement();
        pNode;
        pNode = pNode->NextSiblingElement())
    {
        double restitution = 0.0;
        double friction = 0.0;
        restitution = pNode->DoubleAttribute("restitution");
        friction = pNode->DoubleAttribute("friction");
        m_materialTable.insert(
            std::make_pair(
                pNode->Value(),
                MaterialData((float)restitution,(float)friction)
            )
        );
    }

    // load all densities
    pParentNode = pRoot->FirstChildElement("DensityTable");
    if (!pParentNode) {
        throw std::runtime_error("BulletPhysics LoadXml failed to find DensityTable");
    }
    for (XMLElement* pNode = pParentNode->FirstChildElement();
        pNode;
        pNode = pNode->NextSiblingElement())
    {
        float density = (float)atof(pNode->FirstChild()->Value());
        m_densityTable.insert(
            std::make_pair(pNode->Value(), density)
        );
    }
}
float BulletPhysics::LookupSpecificGravity(const std::string& densityStr)
{
    float density = 0.0f;
    auto densityIt = m_densityTable.find(densityStr);
    if (densityIt != m_densityTable.end()) {
        density = densityIt->second;
    }/* else {
        // TODO - error
    }*/
    return density;
}
MaterialData BulletPhysics::LookupMaterialData(const std::string& materialStr)
{
    auto materialIt = m_materialTable.find(materialStr);
    if (materialIt != m_materialTable.end()) {
        return materialIt->second;
    }
    return MaterialData(0.0f,0.0f);
}

btRigidBody* BulletPhysics::FindBulletRigidBody(ActorId id) const
{
    ActorIDToBulletRigidBodyMap::const_iterator found =
        m_actorIdToRigidBody.find(id);
    if (found != m_actorIdToRigidBody.end()) {
        return found->second;
    }
    return nullptr;
}

ActorId BulletPhysics::FindActorId(btRigidBody const* body) const
{
    BulletRigidBodyToActorIdMap::const_iterator found =
        m_rigidBodyToActorId.find(body);
    if (found != m_rigidBodyToActorId.end()) {
        return found->second;
    }
    return INVALID_ACTOR_ID;
}

    
// helpers for sending events relating to collision pairs
void BulletPhysics::SendCollisionPairAddEvent(
    btPersistentManifold const* manifold,
    btRigidBody const* body0, btRigidBody const* body1)
{
    if (body0->getUserPointer() || body1->getUserPointer())
    {
        // only triggers have non-null user pointers

        // figure out which actor is the trigger
        btRigidBody const* triggerBody;
        btRigidBody const* otherBody;
        if (body0->getUserPointer()) {
            triggerBody = body0;
            otherBody = body1;
        } else {
            triggerBody = body0;
            otherBody = body1;
        }

        // send the trigger event
        int const triggerId = *static_cast<int*>(triggerBody->getUserPointer());
        std::shared_ptr<EvtDataPhysTriggerEnter> pEvent(
            new EvtDataPhysTriggerEnter(triggerId, FindActorId(otherBody))
        );
        IEventManager::GetInstance()->VQueueEvent(pEvent);
    }
    else
    {
        ActorId const id0 = FindActorId(body0);
        ActorId const id1 = FindActorId(body1);
        if (id0 == INVALID_ACTOR_ID || id1 == INVALID_ACTOR_ID) {
            // something is colliding with a non-actor. we currently don't send events for that.
            return;
        }

        // This pair of colliding objects is new. send a collision-begin event
        Vec3List collisionPoints;
        Vec3 sumNormalForce;
        Vec3 sumFrictionForce;
        for (int pointIdx = 0; pointIdx < manifold->getNumContacts(); ++pointIdx)
        {
            btManifoldPoint const& point = manifold->getContactPoint(pointIdx);
            collisionPoints.push_back(BtVector3ToVec3(point.getPositionWorldOnB()));
            sumNormalForce += BtVector3ToVec3(point.m_combinedRestitution * point.m_normalWorldOnB);
            sumFrictionForce += BtVector3ToVec3(point.m_combinedFriction * point.m_lateralFrictionDir1);
        }

        // send the event for the game
        std::shared_ptr<EvtDataPhysCollision> pEvent(
            new EvtDataPhysCollision(
                id0,
                id1,
                sumNormalForce,
                sumFrictionForce,
                collisionPoints
            )
        );
        IEventManager::GetInstance()->VQueueEvent(pEvent);
    }
}
void BulletPhysics::SendCollisionPairRemoveEvent(
    btRigidBody const* body0, btRigidBody const* body1)
{
    if (body0->getUserPointer() || body1->getUserPointer())
    {
        // figure out which actor is the trigger
        btRigidBody const* triggerBody;
        btRigidBody const* otherBody;
        if (body0->getUserPointer()) {
            triggerBody = body0;
            otherBody = body1;
        } else {
            triggerBody = body1;
            otherBody = body0;
        }

        // send the trigger event
        int const triggerId = *static_cast<int*>(triggerBody->getUserPointer());
        std::shared_ptr<EvtDataPhysTriggerLeave> pEvent(
            new EvtDataPhysTriggerLeave(triggerId, FindActorId(otherBody))
        );
        IEventManager::GetInstance()->VQueueEvent(pEvent);
    }
    else
    {
        ActorId const id0 = FindActorId(body0);
        ActorId const id1 = FindActorId(body1);

        if (id0 == INVALID_ACTOR_ID || id1 == INVALID_ACTOR_ID) {
            // collision is ending between some objects that don't have actors.
            // we don't send events for that.
            return;
        }

        std::shared_ptr<EvtDataPhysSeparation> pEvent(
            new EvtDataPhysSeparation(id0, id1)
        );
        IEventManager::GetInstance()->VQueueEvent(pEvent);
    }
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
        printf("No transform component in actor w/ id %llu\n", (long long unsigned)actorId);
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
    // first remove the object from the physics sim
    m_pDynamicsWorld->removeCollisionObject(removeMe);

    // then remove the pointer from the ongoing contacts list
    for (CollisionPairs::iterator it = m_previousTickCollisionPairs.begin();
        it != m_previousTickCollisionPairs.end();
        )
    {
        CollisionPairs::iterator next = it;
        ++next;

        if (it->first == removeMe || it->second == removeMe) {
            SendCollisionPairRemoveEvent(it->first, it->second);
            m_previousTickCollisionPairs.erase(it);
        }
        it = next;
    }

    // if the object is a RigidBody (all we support, but good practice to check)
    if (btRigidBody* const body = btRigidBody::upcast(removeMe))
    {
        // delete the components of the object
        delete body->getMotionState();
        delete body->getCollisionShape();
        //delete body->getUserPointer();

        for (int ii = body->getNumConstraintRefs()-1; ii >= 0; --ii)
        {
            btTypedConstraint* const pConstraint = body->getConstraintRef(ii);
            m_pDynamicsWorld->removeConstraint(pConstraint);
            delete pConstraint;
        }
    }

    delete removeMe;
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

void BulletPhysics::VRotateY(ActorId actorId, float angleRadians, float time)
{
    //TODO
}

float BulletPhysics::VGetOrientationY(ActorId actorId)
{
    //TODO
    return 0.0f;
}

void BulletPhysics::VStopActor(ActorId actorId)
{
    //TODO
}

Vec3 BulletPhysics::VGetVelocity(ActorId actorId)
{
    //TODO
    return Vec3();
}

void BulletPhysics::VSetVelocity(ActorId actorId, const Vec3& vel)
{
    //TODO
}

Vec3 BulletPhysics::VGetAngularVelocity(ActorId actorId)
{
    //TODO
    return Vec3();
}

void BulletPhysics::VSetAngularVelocity(ActorId actorId, const Vec3& vel)
{
    //TODO
}

void BulletPhysics::VTranslate(ActorId actorId, const Vec3& vec)
{
    //TODO
}

void BulletPhysics::VSetTransform(const ActorId id, const Mat4x4& mat)
{
    //TODO
}

Mat4x4 BulletPhysics::VGetTransform(const ActorId id)
{
    //TODO
    return Mat4x4();
}

