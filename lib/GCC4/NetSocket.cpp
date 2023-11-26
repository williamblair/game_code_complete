#include "NetSocket.h"
#include "SocketManager.h"
#include "EventManager.h"

NetSocket::NetSocket()
{
    m_sock = INVALID_SOCKET;
    m_deleteFlag = 0;
    m_sendOfs = 0;
    m_timeOut = 0;
    m_recvOfs = m_recvBegin = 0;
    m_internal = 0;
    m_binaryProtocol = true;
}

NetSocket::NetSocket(SOCKET newSock, unsigned int hostIp)
{
    // set everything to zero
    m_deleteFlag = 0;
    m_sendOfs = 0;
    m_timeOut = 0;
    m_recvOfs = m_recvBegin = 0;
    m_internal = 0;

    // check the time
    m_timeCreated = GCC4Time::timeGetTime();

    m_sock = newSock;
    m_ipaddr = hostIp;

    // ask the socket manager if the socket is in our internal network
    m_internal = g_pSocketManager->IsInternal(m_ipaddr);

#ifdef GCC4_USE_WIN_SOCKET
    setsockopt(m_sock, SOL_SOCKET, SO_DONTLINGER, nullptr, 0);
#endif
}

NetSocket::~NetSocket()
{
    if (m_sock != INVALID_SOCKET) {
        //closesocket(m_sock);
        GCC4_CLOSE_SOCKET(m_sock);
        m_sock = INVALID_SOCKET;
    }
}

bool NetSocket::Connect(
    unsigned int ip,
    unsigned int port,
    bool forceCoalesce)
{
    struct sockaddr_in sa;
    int x = 1;

    // create the socket handle
    if ((m_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        return false;
    }

    // set socket options - turn off Nagle alg. if desired
    if (!forceCoalesce) {
        setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&x, sizeof(x));
    }

    // last step - set the IP address/port of the server and connect
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(ip);
    sa.sin_port = htons(port);

    if (connect(m_sock, (struct sockaddr*)&sa, sizeof(sa))) {
        //closesocket(m_sock);
        GCC4_CLOSE_SOCKET(m_sock);
        m_sock = INVALID_SOCKET;
        return false;
    }
    return true;
}

void NetSocket::SetBlocking(bool blocking)
{
#ifdef GCC4_USE_WIN_SOCKET
    unsigned long val = blocking ? 0 : 1;
    ioctlsocket(m_sock, FIONBIO, &val);
#endif
#ifdef GCC4_USE_POSIX_SOCKET
    int curFlags = fcntl(m_sock, F_GETFL);
    if (blocking) {
        curFlags |= O_NONBLOCK;
    } else {
        curFlags &= ~O_NONBLOCK;
    }
    fcntl(m_sock, F_SETFL, curFlags);
#endif
}

void NetSocket::Send(IPacketPtr pPkt, bool clearTimeout)
{
    if (clearTimeout) {
        m_timeOut = 0;
    }
    m_OutList.push_back(pPkt);
}

void NetSocket::VHandleOutput()
{
    int fSent = 0;
    do
    {
        assert(!m_OutList.empty());
        auto i = m_OutList.begin();
        IPacketPtr pPkt = *i;
        const char* pBuf = pPkt->VGetData();
        int len = static_cast<int>(pPkt->VGetSize());
        int rc = send(m_sock, pBuf+m_sendOfs, len-m_sendOfs, 0);
        if (rc > 0) {
            g_pSocketManager->AddToOutbound(rc);
            m_sendOfs += rc;
            fSent = 1;
        }
#ifdef GCC4_USE_WIN_SOCKET
        else if (WSAGetLastError() != WSAEWOULDBLOCK) {
            HandleException();
            fSent = 0;
        }
#endif
#ifdef GCC4_USE_POSIX_SOCKET
        else if (errno == EWOULDBLOCK) {
            HandleException();
            fSent = 0;
        }
#endif
        else {
            fSent = 0;
        }

        if (m_sendOfs == pPkt->VGetSize()) {
            m_OutList.pop_front();
            m_sendOfs = 0;
        }

    } while (fSent && !m_OutList.empty());
}


void NetSocket::VHandleInput()
{
    bool bPktReceived = false;
    unsigned long packetSize = 0;
    int rc = recv(
        m_sock,
        m_recvBuff + m_recvBegin + m_recvOfs,
        RECV_BUFFER_SIZE - (m_recvBegin+m_recvOfs),
        0
    );
    if (rc == 0) {
        return;
    }

    if (rc == SOCKET_ERROR) {
        m_deleteFlag = 1;
        return;
    }

    const int hdrSize = sizeof(unsigned long);
    unsigned int newData = m_recvOfs + rc;
    int processedData = 0;

    while (newData > hdrSize)
    {
        packetSize = *(reinterpret_cast<unsigned long*>(m_recvBuff+m_recvBegin));
        packetSize = ntohl(packetSize);

        // we don't have enough new data to grab the next packet
        if (newData < packetSize) {
            break;
        }

        if (packetSize > MAX_PACKET_SIZE) {
            HandleException();
            return;
        }

        if (newData >= packetSize) {
            // we know how big the packet is and have the full thing
            std::shared_ptr<BinaryPacket> pPkt(
                new BinaryPacket(
                    &m_recvBuff[m_recvBegin+hdrSize],
                    packetSize-hdrSize
                )
            );
            m_InList.push_back(pPkt);
            bPktReceived = true;
            processedData += packetSize;
            newData -= packetSize;
            m_recvBegin += packetSize;
        }
    }

    g_pSocketManager->AddToInbound(rc);
    m_recvOfs = newData;

    if (bPktReceived) {
        if (m_recvOfs == 0) {
            m_recvBegin = 0;
        } else if (m_recvBegin + m_recvOfs + MAX_PACKET_SIZE > 
                RECV_BUFFER_SIZE) {
            // we don't want to overrun the buffer - so we copy the leftover
            // bits to the beginning of the receive buffer and start over
            int leftover = m_recvOfs;
            memcpy(m_recvBuff, &m_recvBuff[m_recvBegin], m_recvOfs);
            m_recvBegin = 0;
        }
    }
}

void NetListenSocket::Init(int portnum)
{
    struct sockaddr_in sa;
    int value = 1;

    // create socket handle
    if ((m_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("NetListenSocketError: Init failed to create socket handle\n");
        return;
    }

    // set socket options to reuse server socket addresses even if they are
    // busy - this is important if your server restarts and you don't want
    // to wait for your sockets to time out.
    if (setsockopt(
            m_sock,
            SOL_SOCKET,
            SO_REUSEADDR,
            (char*)&value,
            sizeof(value)) == SOCKET_ERROR) {
        GCC4_CLOSE_SOCKET(m_sock);
        m_sock = INVALID_SOCKET;
        printf("NetListenSocket error: init failed to set socket options\n");
        return;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = ADDR_ANY;
    sa.sin_port = htons(portnum);

    // bind to port
    if (bind(m_sock, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
        GCC4_CLOSE_SOCKET(m_sock);
        m_sock = INVALID_SOCKET;
        printf("NetListenSocket error: init failed to bind\n");
    }

    // set nonblocking -
    // accept() blocks under some odd circumstances otherwise
    SetBlocking(false);

    // start listening
    if (listen(m_sock, 256) == SOCKET_ERROR) {
        GCC4_CLOSE_SOCKET(m_sock);
        m_sock = INVALID_SOCKET;
        printf("NetListenSocket error: init failed to listen\n");
        return;
    }

    port = portnum;
}

SOCKET NetListenSocket::AcceptConnection(unsigned int* pAddr)
{
    SOCKET newSock;
    struct sockaddr_in sock;
    int size = sizeof(sock);

    if ((newSock = accept(m_sock, (struct sockaddr*)&sock, &size)) == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }

    if (getpeername(newSock, (struct sockaddr*)&sock, &size) == SOCKET_ERROR) {
        GCC4_CLOSE_SOCKET(newSock);
        return INVALID_SOCKET;
    }
    *pAddr = ntohl(sock.sin_addr.s_addr);
    return newSock;
}




void GameServerListenSocket::VHandleInput()
{
    unsigned int theipaddr;
    SOCKET newSock = AcceptConnection(&theipaddr);

    int value = 1;
    setsockopt(newSock, SOL_SOCKET, SO_DONTLINGER, (char*)&value, sizeof(value));

    if (newSock != INVALID_SOCKET) {
        RemoteEventSocket* pSock = new RemoteEventSocket(newSock, theipaddr);
        int sockid = g_pSocketManager->AddSocket(pSock);
        int ipAddress = g_pSocketManager->GetIpAddress(sockid);
        std::shared_ptr<EvtDataRemoteClient> pEvent(
            new EvtDataRemoteClient(sockid, ipAddress)
        );
        IEventManager::GetInstance()->VQueueEvent(pEvent);
    }
}

void RemoteEventSocket::VHandleInput()
{
    NetSocket::VHandleInput();

    // traverse the list of m_InList packets and do something with them
    while (!m_InList.empty())
    {
        IPacketPtr pPacket = *m_InList.begin();
        m_InList.pop_front();
        const char* pBuf = pPacket->VGetData();
        int size = static_cast<int>(pPacket->VGetSize());

        std::istringstream in(pBuf+sizeof(unsigned long), (size - sizeof(unsigned long)));

        int type;
        in >> type;
        switch (type)
        {
        case NetMsg_Event:
            printf("RemoteEventSocket NetMsg_Event\n");
            CreateEvent(in);
            break;
        case NetMsg_PlayerLoginOk:
        {
            printf("RemoteEventSocket NetMsg_PlayerLoginOk\n");
            int serverSockId, actorId;
            in >> serverSockId;
            in >> actorId;
            std::shared_ptr<EvtDataNetworkPlayerActorAssignment> pEvent(
                new EvtDataNetworkPlayerActorAssignment(
                    actorId, serverSockId
                )
            );
            IEventManager::GetInstance()->VQueueEvent(pEvent);
            break;
        }
        default:
            printf("Unknown message type!\n");
            break;
        }
    }
}

void RemoteEventSocket::CreateEvent(std::istringstream& in)
{
    // TODO - CREATE_EVENT and event factory
    /*
    EventType eventType;
    in >> eventType;
    IEventDataPtr pEvent(CREATE_EVENT(eventType));
    if (pEvent) {
        pEvent->VDeserialize(in);
        IEventManager::Get()->VQueueEvent(pEvent);
    } else {
        printf("ERROR unknown event type from remote: 0x %s\n",
            ToStr(eventType, 16));
    }*/
    printf("UNIMPLEMENTED - RemoteEventSocket::CreateEvent!!!\n");
}

void NetworkEventForwarder::ForwardEvent(IEventDataPtr pEventData)
{
    std::ostringstream out;
    out << static_cast<int>(RemoteEventSocket::NetMsg_Event) << " ";
    out << pEventData->VGetEventType() << " ";
    pEventData->VSerialize(out);
    out << "\r\n";
    IPacketPtr eventMsg(
        new BinaryPacket(out.str().c_str(), out.str().size())
    );
    g_pSocketManager->Send(m_sockId, eventMsg);
}

