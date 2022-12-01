#ifndef GCC4_IGAMEPHYSICS_H_INCLUDED
#define GCC4_IGAMEPHYSICS_H_INCLUDED

#include <string>

#include "GCCMath.h"
#include "ComponentTypes.h"

class IGamePhysics
{
public:

    // Initialization and maintenance of the physics world
    virtual bool VInitialize() = 0;
    virtual void VOnUpdate(float deltaSeconds) = 0;
    
    virtual void VSyncVisibleScene() = 0;
    
    // Initialization of physics objects
    virtual void VAddSphere(
        float radius,
        WeakActorPtr pGameActor,
        //const Mat4x4& initialTransform,
        const std::string& densityStr,
        const std::string& physicsMaterial
    ) = 0;
    virtual void VAddBox(
        const Vec3& dimensions,
        WeakActorPtr pGameActor,
        //const Mat4x4& initialTransform,
        const std::string& densityStr,
        const std::string& physicsMaterial
    ) = 0;
    virtual void VRemoveActor(ActorId id) = 0;
    
    // Debugging
    virtual void VRenderDiagnostics() = 0;
    
    // Physics world modifiers
    virtual void VCreateTrigger(
        WeakActorPtr actor,
        const Vec3& pos,
        const float dim
    ) = 0;
    virtual void VApplyForce(const Vec3& dir, float newtons, ActorId id) = 0;
    virtual void VApplyTorque(const Vec3& dir, float newtons, ActorId id) = 0;
    virtual bool VKinematicMove(const Mat4x4& mat, ActorId id) = 0;
};

#endif // GCC4_IGAMEPHYSICS_H_INCLUDED
