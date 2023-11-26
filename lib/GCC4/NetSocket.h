#ifndef GCC4_NET_SOCKET_H_INCLUDED
#define GCC4_NET_SOCKET_H_INCLUDED

#include <list>
#include <sstream>

#include "GCCTime.h"
#include "IPacket.h"
#include "EventData.h"

#define MAX_PACKET_SIZE (256)
#define RECV_BUFFER_SIZE (MAX_PACKET_SIZE * 512)

class NetSocket
{
public:
    NetSocket();
    NetSocket(SOCKET newSock, unsigned int hostIp);
    virtual ~NetSocket();

    bool Connect(unsigned int ip, unsigned int port, bool forceCoalesce = false);
    void SetBlocking(bool blocking);
    void Send(IPacketPtr pPkt, bool clearTimeOut = true);

    virtual int VHasOutput() { return !m_OutList.empty(); }
    virtual void VHandleOutput();
    virtual void VHandleInput();
    virtual void VTimeOut() { m_timeOut = 0; }

    void HandleException() { m_deleteFlag |= 1; }
    void SetTimeOut(unsigned int ms = 45*1000) { m_timeOut = GCC4Time::timeGetTime()+ms; }
    int GetIpAddress() const { return m_ipaddr; }

protected:

    friend class BaseSocketManager;

    SOCKET m_sock; // socket handle
    int m_id; // a unique ID given by the socket manager

    // if deleteFlag has bit 2 set, exceptions only close the
    // socket and set to INVALID_SOCKET, and do not delete the NetSocket
    int m_deleteFlag;

    typedef std::list<IPacketPtr> PacketList;
    PacketList m_OutList; // packets to send
    PacketList m_InList; // packets just received

    char m_recvBuff[RECV_BUFFER_SIZE];
    unsigned int m_recvOfs, m_recvBegin; // tracking read head of the buffer

    int m_sendOfs; // tracking output buffer
    unsigned int m_timeOut; // when will the socket time out
    unsigned int m_ipaddr; // ip address of the remote connection

    int m_internal; // is the remote ip internal or external?
    int m_timeCreated; // when the socket was created
    bool m_binaryProtocol; // using binary or not?
};

class NetListenSocket : public NetSocket
{
public:
    NetListenSocket() {}
    NetListenSocket(int portnum) { port = 0; Init(portnum); }

    void Init(int portnum);
    SOCKET AcceptConnection(unsigned int* pAddr);

    unsigned short port;
};

class GameServerListenSocket : public NetListenSocket
{
public:
    GameServerListenSocket(int portnum) { Init(portnum); }
    void VHandleInput();
};

class RemoteEventSocket : public NetSocket
{
public:
    enum
    {
        NetMsg_Event,
        NetMsg_PlayerLoginOk
    };

    // server accepting a client
    RemoteEventSocket(SOCKET newSock, unsigned int hostIp) :
        NetSocket(newSock, hostIp)
    {}

    // client attach to server
    RemoteEventSocket() {}

    virtual void VHandleInput();

protected:
    void CreateEvent(std::istringstream& in);
};

class NetworkEventForwarder
{
public:
    NetworkEventForwarder(int sockId) { m_sockId = sockId; }
    void ForwardEvent(IEventDataPtr pEventData);
protected:
    int m_sockId;
};

#endif // GCC4_NET_SOCKET_H_INCLUDED

