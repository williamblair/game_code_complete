#include <string>
#include <map>
#include <cassert>
#include <chrono>
#include <thread>

#include <GCC4/EventData.h>
#include <GCC4/EventManager.h>

#define ACTOR_ID 1

class RoleSystem
{
public:

    RoleSystem() :
        destroyDelegate(
            DECL_MBR_DELEGATE(&RoleSystem::DestroyActorDelegate))
    {
        ActorId actorId = ActorId(ACTOR_ID);
        m_roleMap[actorId] = "Actor1";

        IEventManager* evtMgr = IEventManager::GetInstance();
        assert(evtMgr != nullptr);
        evtMgr->VAddListener(destroyDelegate, EvtDataDestroyActor::sk_EventType);
    }
    ~RoleSystem()
    {
        IEventManager* evtMgr = IEventManager::GetInstance();
        evtMgr->VRemoveListener(destroyDelegate, EvtDataDestroyActor::sk_EventType);
    }

    void DestroyActorDelegate( IEventDataPtr pEventData )
    {
        // cast pointer to the actual event data
        // important to properly cast so the shared_ptr<> isn't
        // deleted prematurely before we're done using it
        std::shared_ptr<EvtDataDestroyActor> pCastEventData(
            std::static_pointer_cast<EvtDataDestroyActor>(pEventData)
        );

        std::cout << "RoleSystem attempting to remove actor" << std::endl;
        auto it = m_roleMap.find(pCastEventData->GetId());
        if (it != m_roleMap.end()) {
            std::cout << "  found actor: " << it->second << std::endl;
            m_roleMap.erase(it);
        } else {
            std::cout << "  failed to find actor" << std::endl;
        }
    }

private:

    std::map<ActorId, std::string> m_roleMap;
    EventListenerDelegate destroyDelegate;
};

int main(void)
{
    std::unique_ptr<EventManager> pEventMgr(
        new EventManager("EventManager", true) // set this as the global instance
    );
    std::unique_ptr<RoleSystem> pRoleSystem(
        new RoleSystem()
    );

    bool eventFired = false;

    auto timeStart = std::chrono::steady_clock::now();
    auto curTime = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
                curTime - timeStart).count() < 1000*10)
    {
        curTime = std::chrono::steady_clock::now();

        // event to remove actor after 5 seconds
        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                curTime - timeStart).count() > 1000 * 5 &&
            !eventFired)
        {
            IEventDataPtr evt(new EvtDataDestroyActor(ActorId(ACTOR_ID)));
            pEventMgr->VQueueEvent( evt );
            eventFired = true;
        }

        pEventMgr->VTickUpdate();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }

    return 0;
}

