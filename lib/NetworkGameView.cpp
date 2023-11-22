#include "NetworkGameView.h"

NetworkGameView::NetworkGameView()
{
    m_SockId = -1;
    m_ActorId = INVALID_ACTOR_ID;
    IEventManager::Get()->VAddListener(
        MakeDelegate(this, &NetworkGameView::NewActorDelegate),
        EvtDataNewActor::sk_EventType
    );
}

void NetworkGameView::AttachRemotePlayer(int sockId)
{
    m_SockId = sockId;
    std::ostrstream out;
    out << static_cast<int>(RemoteEventSocket::NetMsg_PlayerLoginOk) << " ";
    out << m_SockId << " ";
    out << m_ActorId << " ";
    out << "\r\n";

    IPacketPtr gvidMsg(
        new BinaryPacket(out.rdbuf()->str(), (unsigned long)out.pcount())
    );
    g_pSocketManager->Send(m_SockId, gvidMsg);
}

