#ifndef GCC4_IPACKET_H_INCLUDED
#define GCC4_IPACKET_H_INCLUDED

#include <memory>

#ifdef _WIN32
#define GCC4_USE_WIN_SOCKET
#else
#define GCC4_USE_POSIX_SOCKET
#endif

#ifdef GCC4_USE_POSIX_SOCKET
#include <assert.h>
#include <string.h> // memcpy
#include <arpa/inet.h> // htonl
#endif

#ifdef GCC4_USE_WIN_SOCKET
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

// packet interface
class IPacket
{
public:
    virtual char const* const VGetType() const = 0;
    virtual char const* const VGetData() const = 0;
    virtual unsigned long VGetSize() const = 0;
    virtual ~IPacket() {}
};
typedef std::shared_ptr<IPacket> IPacketPtr;

// binary stored packet
class BinaryPacket : public IPacket
{
public:
    inline BinaryPacket(char const* const pData, unsigned long size);
    inline BinaryPacket(unsigned long size);

    virtual ~BinaryPacket() { delete[] m_pData; }
    virtual char const* const VGetData() const { return m_pData; }
    virtual unsigned long VGetSize() const { return ntohl(*(unsigned long*)m_pData); }

    inline void MemCpy(char const* const pData, size_t size, int destOffset);
protected:
    char* m_pData;
};

inline BinaryPacket::BinaryPacket(char const* const pData, unsigned long size)
{
    m_pData = new char[size+sizeof(unsigned long)];
    assert(m_pData);
    *(unsigned long*)m_pData = htonl(size+sizeof(unsigned long));
    memcpy(m_pData + sizeof(unsigned long), pData, size);
}

inline BinaryPacket::BinaryPacket(unsigned long size)
{
    m_pData = new char[size+sizeof(unsigned long)];
    assert(m_pData);
    *(unsigned long*)m_pData = htonl(size+sizeof(unsigned long));
}

inline void BinaryPacket::MemCpy(char const* const pData, size_t size, int destOffset)
{
    assert(size+destOffset <= VGetSize()-sizeof(unsigned long));
    memcpy(m_pData + destOffset + sizeof(unsigned long), pData, size);
}

#endif // GCC4_IPACKET_H_INCLUDED

