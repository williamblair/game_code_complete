#ifndef GCC4_IGAMELOGIC_H_INCLUDED
#define GCC4_IGAMELOGIC_H_INCLUDED

#include "ComponentTypes.h"
#include "xml.h"
#include "GCCMath.h"

// forward declaration
class IGamePhysics;

class IGameLogic
{
public:
    virtual WeakActorPtr VGetActor(const ActorId id) = 0;
    virtual StrongActorPtr VCreateActor(
        const std::string& actorResource,
        XMLElement* overrides,
        const Mat4x4* initialTransform = nullptr,
        const ActorId serversActorId = INVALID_ACTOR_ID
    ) = 0;
    virtual void VDestroyActor(const ActorId actorId) = 0;
    virtual bool VLoadGame(const char* levelResource) = 0;
    virtual void VSetProxy() = 0;
    virtual void VOnUpdate(float time, float elapsedTime) = 0;
    virtual void VChangeState(enum BaseGameState newState) = 0;
    virtual void VMoveActor(const ActorId id, Mat4x4 const& mat) = 0;
    virtual std::shared_ptr<IGamePhysics> VGetGamePhysics() = 0;
};

#endif // GCC4_IGAMELOGIC_H_INCLUDED
