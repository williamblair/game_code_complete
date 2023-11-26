#ifndef GCC4_ASSIMP_MESH_RESOURCE_EXTRADATA_H_INCLUDED
#define GCC4_ASSIMP_MESH_RESOURCE_EXTRADATA_H_INCLUDED

#include "IResourceExtraData.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

class AssimpMeshResourceExtraData : public IResourceExtraData
{
    friend class AssimpMeshResourceLoader;
public:
    AssimpMeshResourceExtraData() :
        m_pScene(nullptr) {}
    virtual ~AssimpMeshResourceExtraData() {}
    virtual std::string VToString() { return "AssimpMeshResourceExtraData"; }
    
    const aiScene* GetScene() { return m_pScene; }
private:
    Assimp::Importer m_Importer;
    const aiScene* m_pScene;
};

#endif // GCC4_ASSIMP_MESH_RESOURCE_EXTRADATA_H_INCLUDED
