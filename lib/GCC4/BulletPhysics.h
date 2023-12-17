#ifndef GCC4_BULLET_PHYSICS_H_INCLUDED
#define GCC4_BULLET_PHYSICS_H_INCLUDED

#include <map>
#include <string>
#include <set>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "MaterialData.h"
#include "IGamePhysics.h"
#include "TransformComponent.h"

// TODO
class BulletDebugDrawer;

class BulletPhysics : public IGamePhysics
{
public:

    BulletPhysics();
    virtual ~BulletPhysics();
    
    // Initialization and maintenance of the physics world
    virtual bool VInitialize() override;
    virtual void VSyncVisibleScene() override;
    virtual void VOnUpdate(float deltaSeconds) override;
    
    // Initialization of physics objects
    virtual void VAddSphere(
        float radius,
        WeakActorPtr pGameActor,
        //const Mat4x4& initialTransform,
        const std::string& densityStr,
        const std::string& physicsMaterial
    ) override;
    virtual void VAddBox(
        const Vec3& dimensions,
        WeakActorPtr pGameActor,
        //const Mat4x4& initialTransform,
        const std::string& densityStr,
        const std::string& physicsMaterial
    ) override;
    virtual void VRemoveActor(ActorId id) override;
    
    // Debugging
    virtual void VRenderDiagnostics() override;
    
    // Physics world modifiers
    virtual void VCreateTrigger(
        WeakActorPtr pActor,
        const Vec3& pos,
        const float dim
    ) override;
    virtual void VApplyForce(const Vec3& dir, float newtons, ActorId id) override;
    virtual void VApplyTorque(const Vec3& dir, float newtons, ActorId id) override;
    virtual bool VKinematicMove(const Mat4x4& mat, ActorId id) override;

private:
    // Bullet library vars
    btDynamicsWorld* m_pDynamicsWorld;
    btBroadphaseInterface* m_pBroadPhase;
    btCollisionDispatcher* m_pDispatcher;
    btConstraintSolver* m_pSolver;
    btDefaultCollisionConfiguration* m_pCollisionConfiguration;
    BulletDebugDrawer* m_pDebugDrawer;
    
    // Tables read from XML
    typedef std::map<std::string,float> DensityTable;
    typedef std::map<std::string,MaterialData> MaterialTable;
    DensityTable m_densityTable;
    MaterialTable m_materialTable;
    void LoadXml();
    float LookupSpecificGravity(const std::string& densityStr);
    MaterialData LookupMaterialData(const std::string& materialStr);
    
    // keep track of existing rigid bodies to check them for updates
    // to the actors positions, and to remove them when their lives are over.
    typedef std::map<ActorId,btRigidBody*> ActorIDToBulletRigidBodyMap;
    ActorIDToBulletRigidBodyMap m_actorIdToRigidBody;
    btRigidBody* FindBulletRigidBody(ActorId id) const;
    
    // also keep a map to get the actor id from the btRigidBody*
    typedef std::map<btRigidBody const*,ActorId> BulletRigidBodyToActorIdMap;
    BulletRigidBodyToActorIdMap m_rigidBodyToActorId;
    ActorId FindActorId(btRigidBody const*) const;
    
    // Data used to store which collision pair (bodies that are touching) need
    // collision events sent. When a new pair of touching bodies are detected,
    // they are added to m_previousTickCollisionPairs and an event is sent.
    // When the pair is no longer needed, they are removed and another event is set.
    typedef std::pair<btRigidBody const*,btRigidBody const*> CollisionPair;
    typedef std::set<CollisionPair> CollisionPairs;
    CollisionPairs m_previousTickCollisionPairs;
    
    // helpers for sending events relating to collision pairs
    void SendCollisionPairAddEvent(
        btPersistentManifold const* manifold,
        btRigidBody const* body0, btRigidBody const* body1
    );
    void SendCollisionPairRemoveEvent(
        btRigidBody const* body0, btRigidBody const* body1
    );
    
    // common functionality used by VAddSphere, VAddBox, etc.
    void AddShape(
        StrongActorPtr pGameActor,
        btCollisionShape* shape,
        float mass,
        const std::string& physicsMaterial
    );
    
    // helper for cleaning up objects
    void RemoveCollisionObject(btCollisionObject* removeMe);
    
    // callback from bullet for each physics time step. set in VInitialize
    static void BulletInternalTickCallback(
        btDynamicsWorld* const pWorld,
        btScalar const timeStep
    );
};

#endif // GCC4_BULLET_PHYSICS_H_INCLUDED

