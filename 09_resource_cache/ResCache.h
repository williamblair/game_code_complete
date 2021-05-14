#ifndef RES_CACHE_H_INCLUDED
#define RES_CACHE_H_INCLUDED

#include <ResCacheTypes.h>

// forward declaration
class IResourceFile;

class ResCache
{
public:

    ResCache( const unsigned int sizeInMb, IResourceFile* pResFile );
    ~ResCache();
    
    bool Init();
    void RegisterLoader( std::shared_ptr<IResourceLoader> pLoader );
    
    std::shared_ptr<ResHandle> GetHandle( Resource* pResource );
    int Preload( const std::string pattern, 
                 void (*progressCallback)(int, bool&) );
    void Flush();

protected:

    ResHandleList m_lru; // least recently used list
    ResHandleMap m_resources;
    ResourceLoaders m_resourceLoaders;
    
    IResourceFile* m_pFile;
    
    unsigned int m_cacheSize; // total memory size
    unsigned int m_allocated; // total memory allocated
    
    std::shared_ptr<ResHandle> Find( Resource* pResource );
    const void* Update( std::shared_ptr<ResHandle> pHandle );
    std::shared_ptr<ResHandle> Load( Resource* pResource );
    void Free( std::shared_ptr<ResHandle> pGonner );
    
    bool MakeRoom( unsigned int size );
    char* Allocate( unsigned int size );
    void FreeOneResource();
    void MemoryHasBeenFreed( unsigned int size );

};

#endif // RES_CACHE_H_INCLUDED
