#include <GCC4/EventData.h>
#include <GCC4/EventManager.h>
#include "TeapotEvents.h"
#include "TeapotController.h"

#define ACTOR_ACCELERATION (6.5f * 8.0f)
#define ACTOR_ANGULAR_ACCELERATION (22.0f)

TeapotController::TeapotController(std::shared_ptr<SceneNode> object) :
    m_object(object)
{
    memset(m_bKey, 0x00, sizeof(m_bKey));
}


bool TeapotController::VOnPointerButtonDown(const Point& mousePos, const int radius, const std::string& buttonName)
{
    if (buttonName != "PointerLeft") {
        return false;
    }

    ActorId actorId = m_object->VGet()->GetActorId();
    assert(actorId != INVALID_ACTOR_ID && "The teapot controller isnt attached to a valid actor!");
    std::shared_ptr<EvtDataFireWeapon> pFireEvent(new EvtDataFireWeapon(actorId));
    IEventManager::GetInstance()->VQueueEvent(pFireEvent);
    return true;
}

bool TeapotController::VOnKeyDown(const char c)
{
    m_bKey[size_t(c)] = true;

    // send thrust event
    if (c == 'W' || c == 'S') {
        const ActorId actorId = m_object->VGet()->GetActorId();
        std::shared_ptr<EvtDataStartThrust> pEvent(
            new EvtDataStartThrust(
                actorId,
                c == 'W' ? ACTOR_ACCELERATION : -ACTOR_ACCELERATION
            )
        );
    }

    // send steer event
    if (c == 'A' || c == 'D') {
        const ActorId actorId = m_object->VGet()->GetActorId();
        std::shared_ptr<EvtDataStartSteer> pEvent(
            new EvtDataStartSteer(
                actorId,
                c == 'D' ? ACTOR_ANGULAR_ACCELERATION : -ACTOR_ANGULAR_ACCELERATION
            )
        );
        IEventManager::GetInstance()->VQueueEvent(pEvent);
    }

    return true;
}

bool TeapotController::VOnKeyUp(const char c)
{
    m_bKey[size_t(c)] = false;

    // send end thrust event
    if (c == 'W' || c == 'S') {
        const ActorId actorId = m_object->VGet()->GetActorId();
        std::shared_ptr<EvtDataEndThrust> pEvent(
            new EvtDataEndThrust(actorId)
        );
    }

    // send steer event
    if (c == 'A' || c == 'D') {
        const ActorId actorId = m_object->VGet()->GetActorId();
        std::shared_ptr<EvtDataEndSteer> pEvent(
            new EvtDataEndSteer(actorId)
        );
        IEventManager::GetInstance()->VQueueEvent(pEvent);
    }

    return true;
}

void TeapotController::OnUpdate(unsigned long elapsedMs)
{
    // does nothing
}

