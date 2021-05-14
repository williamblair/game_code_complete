#include <ResHandle.h>

ResHandle::ResHandle( Resource& resource,
                      char* pBuffer,
                      unsigned int size,
                      ResCache* pResCache )
{
    m_pBuffer = pBuffer;
    m_size = size;
    m_pResCache = pResCache;
}

ResHandle::~ResHandle()
{
    delete[] m_pBuffer;
    m_pResCache->MemoryHasBeenFreed( m_size );
}

