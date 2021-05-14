#include <ResCache.h>
#include <DefaultResourceLoader.h>

ResCache::ResCache( const unsigned int sizeInMb, IResourceFile* pResFile )
{
    m_cacheSize = sizeInMb * 1024 * 1024;
    m_allocated = 0;
    m_pFile = pResFile;
}

ResCache::~ResCache()
{
    while ( !m_lru.empty() )
    {
        FreeOneResource();
    }
    
    delete m_file;
}


bool ResCache::Init()
{
    bool retVal = false;
    if ( m_pFile->VOpen() )
    {
        RegisterLoader( std::shared_ptr<IResourceLoader>(
            new DefaultResourceLoader() ) );
        retVal = true;
    }
    
    return retVal;
}

std::shared_ptr<ResHandle> ResCache::GetHandle( Resource* pResource )
{
    std::shared_ptr<ResHandle> pHandle( Find( pResource ) );
    // only load if it hasn't been loaded before
    if ( pHandle == nullptr )
    {
        pHandle = Load( pResource );
    }
    else
    {
        // remove the resource from the least-recently-used list, as it has
        // now been used recently
        Update( pHandle );
    }
    
    return pHandle;
}

std::shared_ptr<ResHandle> ResCache::Load( Resource* pResource )
{
    std::shared_ptr<IResourceLoader> pLoader;
    std::shared_ptr<ResHandle> pHandle;
    
    for ( ResourceLoaders::iterator it = m_resourceLoaders.begin();
          it != m_resourceLoaders.end();
          ++it )
    {
        std::shared_ptr<IResourceLoader> testLoader = *it;
        if ( WildcardMatch( testLoader->VGetPattern().c_str(),
             pResource->m_name.c_str() ) )
        {
            pLoader = testLoader;
            break;
        }
    }
    
    if ( !pLoader )
    {
        std::cout << __func__ << ": Default resource loader not found"
                  << std::endl;
        return pHandle; // resource not loaded
    }
    
    unsigned int rawSize = m_pFile->VGetRawResourceSize( *pResource );
    char* pRawBuffer = pLoader->VUseRawFile() ? 
                       Allocate( rawSize ) : 
                       new char[rawSize];
    if ( pRawBuffer == nullptr )
    {
        // resource cache out of memory
        return std::shared_ptr<ResHandle>();
    }
    
    m_pFile->VGetRawResource( *pResource, pRawBuffer );
    char* pBuffer = nullptr;
    unsigned int size = 0;
    
    if ( pLoader->VUseRawFile() )
    {
        pBuffer = pRawBuffer;
        pHandle = std::shared_ptr<ResHandle>( new ResHandle( *pResource,
                                                             pBuffer,
                                                             rawSize,
                                                             this ) );
        ))
    }
    else
    {
        size = pLoader->VGetLoadedResourceSize( pRawBuffer, rawSize );
        pBuffer = Allocate( size );
        if ( pRawBuffer == nullptr || pBuffer == nullptr )
        {
            // resource cache out of memory
            return std::shared_ptr<ResHandle>();
        }
        
        pHandle = std::shared_ptr<ResHandle>( new ResHandle( *pResource,
                                                         pBuffer, 
                                                         size, 
                                                         this ) );
        bool success = pLoader->VLoadResource( pRawBuffer, rawSize, pHandle );
        delete[] pRawBuffer;
    
        if ( !success )
        {
            return std::shared_ptr<ResHandle>();
        }
    }
    
    if ( pHandle )
    {
        m_lru.push_front( pHandle );
        m_resources[ pResource->m_name ] = pHandle;
    }
    
    if ( !pLoader )
    {
        std::cout << __file__ << ": pLoader is null" << std::endl;
        return std::shared_ptr<ResHandle>();
    }
    return pHandle;
}

