#ifndef GCC4_SDK_MESH_RESOURCE_LOADER_H_INCLUDED
#define GCC4_SDK_MESH_RESOURCE_LOADER_H_INCLUDED

#include "IResourceLoader.h"

class SdkMeshResourceLoader : public IResourceLoader
{
public:
    virtual bool VUseRawFile() { return false; }
    virtual bool VDiscardRawBufferAfterLoad() { return false; }
    virtual unsigned int VGetLoadedResourceSize(
        char* rawBuffer, unsigned int rawSize)
    {
        return rawSize;
    }
    virtual bool VLoadResource(
        char* rawBuffer, unsigned int rawSize,
        std::shared_ptr<ResHandle> handle);
    virtual std::string VGetPattern() { return "*.sdkmesh"; }
};

#endif // GCC4_SDK_MESH_RESOURCE_LOADER_H_INCLUDED
