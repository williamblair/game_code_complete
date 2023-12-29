#ifndef GCC4_ASSIMP_MESH_RESOURCE_LOADER_H_INCLUDED
#define GCC4_ASSIMP_MESH_RESOURCE_LOADER_H_INCLUDED

#include "IResourceLoader.h"

class AssimpMeshResourceLoader : public IResourceLoader
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
    virtual std::string VGetPattern() { return ".*\\.(obj|fbx|dae|ply)$"; }
};

std::shared_ptr<IResourceLoader> CreateMeshResourceLoader();

#endif // GCC4_SDK_MESH_RESOURCE_LOADER_H_INCLUDED
