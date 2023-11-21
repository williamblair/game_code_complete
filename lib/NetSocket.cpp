#include "NetSocket.h"

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

    setsockopt(m_sock, SOL_SOCKET, SO_DONTLINGER, nullptr, 0);
}

NetSocket::~NetSocket()
{
    if (m_sock != INVALID_SOCKET) {
        closesocket(m_sock);
        m_socket = INVALID_SOCKET;
    }
}

bool NetSocket::Connect(
    unsigned int ip,
    unsigned int port,
    bool forceCoalesce = false)
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

    if (connect(m_sock, (struct sockaddr*)&sa, sizeof(sa)) {
        closesocket(m_sock);
        m_sock = INVALID_SOCKET;
        return false;
    }
    return true;
}

void NetSocket::SetBlocking(bool blocking)
{
    unsigned long val = blocking ? 0 : 1;
    ioctlsocket(m_sock, FIONBIO, &val);
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
        int rc = send(m_sock, buf.m_sendOfs, len-m_sendOfs, 0);
        if (rc > 0) {
            g_pSocketManager->AddToOutbound(rc);
            m_sendOfs += rc;
            fSent = 1;
        } else if (WSAGetLastError() != WSAEWOULDBLOCK) {
            HandleException();
            fSent = 0;
        } else {
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


