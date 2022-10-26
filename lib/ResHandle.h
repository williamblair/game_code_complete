#ifndef RES_HANDLE_H_INCLUDED
#define RES_HANDLE_H_INCLUDED

#include <memory>
#include "Resource.h"
#include "IResourceExtraData.h"

// forward declaration
class ResCache;

class ResHandle
{

    friend class ResCache;

public:

    ResHandle( Resource& resource,
               char* pBuffer,
               unsigned int size,
               ResCache* pResCache );
    virtual ~ResHandle();

    unsigned int Size() const { return m_size; }
    char* Buffer() const { return m_pBuffer; }
    char* WriteableBuffer() { return m_pBuffer; }
    std::shared_ptr<IResourceExtraData> GetExtra() { return m_pExtra; }
    void SetExtra( std::shared_ptr<IResourceExtraData> extra ) { m_pExtra = extra; }

protected:

    Resource m_resource;
    char* m_pBuffer;
    unsigned int m_size;
    std::shared_ptr<IResourceExtraData> m_pExtra;
    ResCache* m_pResCache;

private:
};

#endif // RES_HANDLE_H_INCLUDED

