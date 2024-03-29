#ifndef GCC4_RES_CACHE_H_INCLUDED
#define GCC4_RES_CACHE_H_INCLUDED

#include <vector>
#include <string>
#include "ResCacheTypes.h"

// forward declaration
class Resource;
class IResourceFile;

class ResCache
{

    friend class ResHandle;

public:

    ResCache(const unsigned int sizeInMb, IResourceFile* pResFile);
    ~ResCache();
    
    bool Init();
    void RegisterLoader(std::shared_ptr<IResourceLoader> pLoader);
    
    std::shared_ptr<ResHandle> GetHandle(Resource* pResource);
    int Preload(
        const std::string pattern, 
        void (*progressCallback)(int, bool&)
    );
    std::vector<std::string> Match(const std::string& pattern);
    void Flush();

protected:

    ResHandleList m_lru; // least recently used list
    ResHandleMap m_resources;
    ResourceLoaders m_resourceLoaders;
    
    IResourceFile* m_pFile;
    
    unsigned int m_cacheSize; // total memory size
    unsigned int m_allocated; // total memory allocated
    
    std::shared_ptr<ResHandle> Find(Resource* pResource);
    void Update(std::shared_ptr<ResHandle> pHandle);
    std::shared_ptr<ResHandle> Load(Resource* pResource);
    void Free(std::shared_ptr<ResHandle> pGonner);
    
    bool MakeRoom(unsigned int size);
    char* Allocate(unsigned int size);
    void FreeOneResource();
    void MemoryHasBeenFreed(unsigned int size);

};

extern ResCache* g_pResCache;

#endif // RES_CACHE_H_INCLUDED
