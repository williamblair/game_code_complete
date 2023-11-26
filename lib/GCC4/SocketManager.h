#ifndef GCC4_SOCKET_MANAGER_H_INCLUDED
#define GCC4_SOCKET_MANAGER_H_INCLUDED

#include <list>
#include <map>
#include <string>

#include "NetSocket.h"

class BaseSocketManager
{
public:
    BaseSocketManager();
    virtual ~BaseSocketManager() { Shutdown(); }

    bool Init();
    void Shutdown();

    int AddSocket(NetSocket* pSocket);
    void RemoveSocket(NetSocket* pSocket);

    bool Send(int sockId, IPacketPtr pPacket);
    void DoSelect(int pauseMicroSecs, int handleInput = 1);

    void SetSubnet(unsigned int subnet, unsigned int subnetMask) {
        m_Subnet = subnet;
        m_SubnetMask = subnetMask;
    }

    bool IsInternal(unsigned int ipaddr);

    unsigned int GetHostByName(const std::string& hostName);
    const char* GetHostByAddr(unsigned int ip);
    
    int GetIpAddress(int sockId);

    void AddToOutbound(int rc) { m_Outbound += rc; }
    void AddToInbound(int rc) { m_Inbound += rc; }

protected:
#ifdef GCC4_USE_WIN_SOCKET
    WSADATA m_WsaData; // describes the sockets system implementation
#endif

    typedef std::list<NetSocket*> SocketList;
    typedef std::map<int, NetSocket*> SocketIdMap;

    SocketList m_SockList;
    SocketIdMap m_SockMap;

    int m_NextSocketId;

    unsigned int m_Inbound; // statistics gathering - inbound data
    unsigned int m_Outbound; // statistics gathering - outbound data
    unsigned int m_MaxOpenSockets; // statistics gathering - max open sockets

    unsigned int m_SubnetMask;
    unsigned int m_Subnet;

    NetSocket* FindSocket(int sockId);
};

class ClientSocketManager : public BaseSocketManager
{
public:
    ClientSocketManager(const std::string& hostName, unsigned int port) {
        m_HostName = hostName;
        m_Port = port;
    }
    bool Connect();
private:
    std::string m_HostName;
    unsigned int m_Port;
};

// located in SocketManager.cpp
extern BaseSocketManager* g_pSocketManager;

#endif // GCC4_SOCKET_MANAGER_H_INCLUDED

