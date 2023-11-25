#include "SocketManager.h"

BaseSocketManager* g_pSocketManager = nullptr;

BaseSocketManager::BaseSocketManager()
{
    m_Inbound = 0;
    m_Outbound = 0;
    m_MaxOpenSockets = 0;
    m_SubnetMask = 0;
    m_Subnet = 0xffffffff;

    g_pSocketManager = this;
#ifdef GCC4_USE_WIN_SOCKET
    memset(&m_WsaData, 0, sizeof(m_WsaData));
#endif
}

bool BaseSocketManager::Init()
{
#ifdef GCC4_USE_WIN_SOCKET
    if (WSAStartup(0x0202, &m_WsaData) == 0) {
        return true;
    } else {
        printf("WSAStartup failure!\n");
        return false;
    }
#endif

    return true;
}

void BaseSocketManager::Shutdown()
{
    while (!m_SockList.empty())
    {
        delete *m_SockList.begin();
        m_SockList.pop_front();
    }

#ifdef GCC4_USE_WIN_SOCKET
    WSACleanup();
#endif
}

int BaseSocketManager::AddSocket(NetSocket* pSocket)
{
    pSocket->m_id = m_NextSocketId;
    m_SockMap[m_NextSocketId] = pSocket;
    ++m_NextSocketId;
    m_SockList.push_front(pSocket);
    if (m_SockList.size() > m_MaxOpenSockets) {
        ++m_MaxOpenSockets;
    }

    return pSocket->m_id;
}

void BaseSocketManager::RemoveSocket(NetSocket* pSocket)
{
    m_SockList.remove(pSocket);
    m_SockMap.erase(pSocket->m_id);
    delete pSocket;
}

NetSocket* BaseSocketManager::FindSocket(int sockId)
{
    auto i = m_SockMap.find(sockId);
    if (i == m_SockMap.end()) {
        return nullptr;
    }

    return i->second;
}

bool BaseSocketManager::Send(int sockId, IPacketPtr pPacket)
{
    NetSocket* pSock = FindSocket(sockId);
    if (!pSock) {
        return false;
    }
    pSock->Send(pPacket);
    return true;
}

void BaseSocketManager::DoSelect(int pauseMicroSecs, int handleInput)
{
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = pauseMicroSecs;

    fd_set inp_set, out_set, exc_set;
    int maxdesc;

    FD_ZERO(&inp_set);
    FD_ZERO(&out_set);
    FD_ZERO(&exc_set);

    maxdesc = 0;

    // setup for select
    for (auto i = m_SockList.begin(); i != m_SockList.end(); ++i)
    {
        NetSocket* pSock = *i;
        if ((pSock->m_deleteFlag & 1) || (pSock->m_sock == INVALID_SOCKET)) {
            continue;
        }

        if (handleInput) {
            FD_SET(pSock->m_sock, &inp_set);
        }
        FD_SET(pSock->m_sock, &exc_set);
        if (pSock->VHasOutput()) {
            FD_SET(pSock->m_sock, &out_set);
        }
        if ((int)pSock->m_sock > maxdesc) {
            maxdesc = (int)pSock->m_sock;
        }
    }

    int selRet = 0;
    // do the select
    // duration passed in as tv, NULL to block until event
    selRet = select(maxdesc+1, &inp_set, &out_set, &exc_set, &tv);
    if (GCC4_SELECT_ERROR(selRet)) {
        printf("Error in DoSelect!\n");
        return;
    }

    // handle input, output, and exceptions
    if (GCC4_SELECT_SUCCESS(selRet)) {
        for (auto i = m_SockList.begin(); i != m_SockList.end(); ++i)
        {
            NetSocket* pSock = *i;
            if ((pSock->m_deleteFlag & 1) || (pSock->m_sock == INVALID_SOCKET)) {
                continue;
            }

            if (FD_ISSET(pSock->m_sock, &exc_set)) {
                pSock->HandleException();
            }

            if (!(pSock->m_deleteFlag & 1) && FD_ISSET(pSock->m_sock, &out_set)) {
                pSock->VHandleOutput();
            }

            if (handleInput &&
                    !(pSock->m_deleteFlag & 1) &&
                    FD_ISSET(pSock->m_sock, &inp_set)) {
                pSock->VHandleInput();
            }
        }
    }

    unsigned int timeNow = GCC4Time::timeGetTime();

    // handle deleting any sockets
    auto i = m_SockList.begin();
    while (i != m_SockList.end())
    {
        NetSocket* pSock = *i;
        if (pSock->m_timeOut && pSock->m_timeOut < timeNow) {
            pSock->VTimeOut();
        }

        if (pSock->m_deleteFlag & 1) {
            switch (pSock->m_deleteFlag)
            {
            case 1:
                g_pSocketManager->RemoveSocket(pSock);
                i = m_SockList.begin();
                break;
            case 3:
                pSock->m_deleteFlag = 2;
                if (pSock->m_sock != INVALID_SOCKET) {
                    GCC4_CLOSE_SOCKET(pSock->m_sock);
                    pSock->m_sock = INVALID_SOCKET;
                }
                break;
            }
        }

        ++i;
    }
}

bool BaseSocketManager::IsInternal(unsigned int ipaddr)
{
    if (!m_SubnetMask) {
        return false;
    }

    if ((ipaddr & m_SubnetMask) == m_Subnet) {
        return false;
    }

    return true;
}

unsigned int BaseSocketManager::GetHostByName(const std::string& hostName)
{
    struct hostent* pHostEnt = gethostbyname(hostName.c_str());
    struct sockaddr_in tmpSockAddr;

    if (pHostEnt == nullptr) {
        printf("GetHostByName error occured\n");
        return 0;
    }

    memcpy(&tmpSockAddr.sin_addr, pHostEnt->h_addr, pHostEnt->h_length);
    return ntohl(tmpSockAddr.sin_addr.s_addr);
}

const char* BaseSocketManager::GetHostByAddr(unsigned int ip)
{
    static char host[32];
    int netip = htonl(ip);
    struct hostent* lpHostEnt = gethostbyaddr((const char*)&netip, 4, PF_INET);
    if (lpHostEnt) {
        strcpy(host, lpHostEnt->h_name);
        return host;
    }

    return nullptr;
}

int BaseSocketManager::GetIpAddress(int sockId)
{
    NetSocket* pSocket = FindSocket(sockId);
    if (pSocket) {
        return pSocket->GetIpAddress();
    }
    return 0;
}


bool ClientSocketManager::Connect()
{
    if (!BaseSocketManager::Init()) {
        return false;
    }

    RemoteEventSocket* pSocket = new RemoteEventSocket;
    assert(pSocket);

    if (!pSocket->Connect(GetHostByName(m_HostName), m_Port)) {
        delete pSocket;
        return false;
    }
    AddSocket(pSocket);
    return true;
}

