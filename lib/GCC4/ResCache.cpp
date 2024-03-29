#include <iostream>
#include <regex>
#include "ResCache.h"
#include "DefaultResourceLoader.h"
#include "IResourceFile.h"
#include "Resource.h"
#include "Logger.h"

ResCache* g_pResCache = nullptr;

ResCache::ResCache(const unsigned int sizeInMb, IResourceFile* pResFile)
{
    m_cacheSize = sizeInMb * 1024 * 1024;
    m_allocated = 0;
    m_pFile = pResFile;
}

ResCache::~ResCache()
{
    while (!m_lru.empty()) {
        FreeOneResource();
    }
    
    delete m_pFile;
}


bool ResCache::Init()
{
    bool retVal = false;
    if (m_pFile->VOpen()) {
        RegisterLoader(std::shared_ptr<IResourceLoader>(
            new DefaultResourceLoader()
        ));
        retVal = true;
    }
    
    return retVal;
}

void ResCache::RegisterLoader(std::shared_ptr<IResourceLoader> pLoader)
{
    m_resourceLoaders.push_front(pLoader);
}

std::shared_ptr<ResHandle> ResCache::GetHandle( Resource* pResource )
{
    std::shared_ptr<ResHandle> pHandle(Find(pResource));
    // only load if it hasn't been loaded before
    if (pHandle == nullptr) {
        pHandle = Load(pResource);
    } else {
        // remove the resource from the least-recently-used list, as it has
        // now been used recently
        Update(pHandle);
    }
    
    return pHandle;
}

int ResCache::Preload(
    const std::string pattern,
    void (*progressCallback)(int, bool&))
{
    //TODO
    return 0;
}

static bool RegexMatch(const char* pattern, const char* expression)
{
    std::regex reg(pattern);
    std::string str(expression);
    return std::regex_match(str, reg);
}

std::vector<std::string> ResCache::Match(const std::string& pattern)
{
    std::vector<std::string> matchingNames;
    if (m_pFile == nullptr) { return matchingNames; }

    int numFiles = m_pFile->VGetNumResources();
    for (int i=0; i<numFiles; ++i) {
        std::string name = m_pFile->VGetResourceName(i);
        std::transform(name.begin(),name.end(),name.begin(), (int(*)(int))std::tolower);
        if (RegexMatch(pattern.c_str(), name.c_str())) {
            matchingNames.push_back(name);
        }
    }
    return matchingNames;
}

std::shared_ptr<ResHandle> ResCache::Load(Resource* pResource)
{
    std::shared_ptr<IResourceLoader> pLoader;
    std::shared_ptr<ResHandle> pHandle;
    
    for (ResourceLoaders::iterator it = m_resourceLoaders.begin();
         it != m_resourceLoaders.end();
         ++it)
    {
        std::shared_ptr<IResourceLoader> testLoader = *it;
        if (RegexMatch(testLoader->VGetPattern().c_str(), pResource->m_name.c_str()))
        {
            pLoader = testLoader;
            break;
        }
    }
    
    if (!pLoader) {
        char buf[256];
        sprintf(buf, "Default resource loader not found: %s", pResource->m_name.c_str());
        GCC_ERROR(buf);
        return pHandle; // resource not loaded
    }
    
    int rawSize = m_pFile->VGetRawResourceSize(*pResource);
    if (rawSize <= 0) {
        char buf[256];
        sprintf(buf, "Get raw resource size returned <= 0 for file: %s", pResource->m_name.c_str());
        GCC_ERROR(buf);
        return pHandle;
    }
    char* pRawBuffer = pLoader->VUseRawFile() ? 
        Allocate(rawSize) : 
        new char[rawSize];
    if (pRawBuffer == nullptr) {
        // resource cache out of memory
        GCC_ERROR("Failed to allocate raw buffer");
        return std::shared_ptr<ResHandle>();
    }
    
    m_pFile->VGetRawResource(*pResource, pRawBuffer);
    char* pBuffer = nullptr;
    unsigned int size = 0;
    
    if (pLoader->VUseRawFile())
    {
        pBuffer = pRawBuffer;
        pHandle = std::shared_ptr<ResHandle>(new ResHandle(
            *pResource,
            pBuffer,
            rawSize,
            this
        ));
    }
    else
    {
        size = pLoader->VGetLoadedResourceSize(pRawBuffer, rawSize);
        pBuffer = Allocate(size);
        if (pRawBuffer == nullptr || pBuffer == nullptr) {
            // resource cache out of memory
            return std::shared_ptr<ResHandle>();
        }
        
        pHandle = std::shared_ptr<ResHandle>(new ResHandle(
            *pResource,
            pBuffer, 
            size, 
            this
        ));
        bool success = pLoader->VLoadResource(pRawBuffer, rawSize, pHandle);
        delete[] pRawBuffer;
    
        if (!success) {
            return std::shared_ptr<ResHandle>();
        }
    }
    
    if (pHandle) {
        m_lru.push_front( pHandle );
        m_resources[ pResource->m_name ] = pHandle;
    }
    
    if (!pLoader) {
        std::cout << __FILE__ << ": pLoader is null" << std::endl;
        return std::shared_ptr<ResHandle>();
    }
    return pHandle;
}

void ResCache::Free(std::shared_ptr<ResHandle> pGonner)
{
    m_lru.remove(pGonner);
    m_resources.erase(pGonner->m_resource.m_name);

    // resource might still be in use by seomthing, so the cache
    // can't actually count the memory freed until the ResHandle
    // pointing to it is destroyed
}

std::shared_ptr<ResHandle> ResCache::Find(Resource* pResource)
{
    ResHandleMap::iterator i = m_resources.find(pResource->m_name);
    if (i == m_resources.end()) {
        return std::shared_ptr<ResHandle>();
    }

    return i->second;
}

void ResCache::Update(std::shared_ptr<ResHandle> pHandle)
{
    // make the resource the front of the most recently used list
    m_lru.remove(pHandle);
    m_lru.push_front(pHandle);
}

bool ResCache::MakeRoom(unsigned int size)
{
    if (size > m_cacheSize) {
        return false;
    }

    // remove some not-recently used entries from the cache to try and free up
    // space
    while (size > (m_cacheSize - m_allocated))
    {
        // the cache is empty and there's still not enough room
        if (m_lru.empty()) {
            return false;
        }
        FreeOneResource();
    }

    return true;
}

char* ResCache::Allocate(unsigned int size)
{
    if (!MakeRoom(size))
    {
        return nullptr;
    }

    char* mem = new char[size];
    if (mem) {
        m_allocated += size;
    }

    return mem;
}

void ResCache::FreeOneResource()
{
    ResHandleList::iterator gonner = m_lru.end();
    --gonner;

    std::shared_ptr<ResHandle> handle = *gonner;

    m_lru.pop_back();
    m_resources.erase(handle->m_resource.m_name);
}

void ResCache::MemoryHasBeenFreed(unsigned int size)
{
    m_allocated -= size;
}

