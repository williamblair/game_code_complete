#ifndef GCC4_TEXTURE_RESOURCE_LOADER_H_INCLUDED
#define GCC4_TEXTURE_RESOURCE_LOADER_H_INCLUDED

#include "IResourceLoader.h"

class TextureResourceLoader : public IResourceLoader
{
public:
    virtual bool VUseRawFile() { return false; }
    virtual bool VDiscardRawbufferAfterLoad() { return true; }
    virtual unsigned int VGetLoadedResourceSize(
        char* pRawBuffer,
        unsigned int rawSize)
    { 
        return 0;
    }
    
    virtual bool VLoadResource(
        char* rawBuffer,
        unsigned int rawSize,
        std::shared_ptr<ResHandle> handle
    );

    virtual std::string VGetPattern() { return ".*\\.(jpg|png|tga|bmp)$"; }
};

std::shared_ptr<IResourceLoader> CreateJPGResourceLoader();

#endif // GCC4_TEXTURE_RESOURCE_LOADER_H_INCLUDED

