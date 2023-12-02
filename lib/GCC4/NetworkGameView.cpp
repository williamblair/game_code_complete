#include "NetworkGameView.h"
#include "MakeStrongPtr.h"
#include "EventManager.h"
#include "GameCodeApp.h"
#include "Actor.h"

NetworkGameView::NetworkGameView()
{
    m_SockId = -1;
    m_ActorId = INVALID_ACTOR_ID;
    m_NewActorDelegate.reset(
        DECL_MBR_DELEGATE(
            &NetworkGameView::NewActorDelegate
        )
    );
    IEventManager::GetInstance()->VAddListener(
        m_NewActorDelegate,
        EvtDataNewActor::sk_EventType
    );
}

void NetworkGameView::VOnUpdate(unsigned long deltaMs)
{
    if (m_ActorId != INVALID_ACTOR_ID) {
        IEventManager::GetInstance()->VRemoveListener(
            m_NewActorDelegate,
            EvtDataNewActor::sk_EventType
        );
    }
}

void NetworkGameView::NewActorDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataNewActor> pCastEventData =
        std::static_pointer_cast<EvtDataNewActor>(pEventData);
    ActorId actorId = pCastEventData->GetActorId();
    StrongActorPtr pActor = MakeStrongPtr(g_pApp->m_pGame->VGetActor(actorId));
    
    // FUTURE work - this could be in a script
    if (pActor && pActor->GetType() == "Teapot")
    {
        if (pCastEventData->GetViewId() == m_ViewId) {
            m_ActorId = actorId;
            std::shared_ptr<EvtDataNetworkPlayerActorAssignment> pEvent(
                new EvtDataNetworkPlayerActorAssignment(
                    m_ActorId,
                    m_SockId
                )
            );
            IEventManager::GetInstance()->VQueueEvent(pEvent);
        }
    }
}

void NetworkGameView::AttachRemotePlayer(int sockId)
{
    m_SockId = sockId;
    std::ostringstream out;
    out << static_cast<int>(RemoteEventSocket::NetMsg_PlayerLoginOk) << " ";
    out << m_SockId << " ";
    out << m_ActorId << " ";
    out << "\r\n";

    IPacketPtr gvidMsg(
        new BinaryPacket(out.str().c_str(), (unsigned long)out.str().size())
    );
    g_pSocketManager->Send(m_SockId, gvidMsg);
}
