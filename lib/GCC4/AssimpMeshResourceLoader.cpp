#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssimpMeshResourceLoader.h"
#include "AssimpMeshResourceExtraData.h"


bool AssimpMeshResourceLoader::VLoadResource(
    char* rawBuffer, unsigned int rawSize,
    std::shared_ptr<ResHandle> handle)
{
    std::shared_ptr<AssimpMeshResourceExtraData> extra =
        std::make_shared<AssimpMeshResourceExtraData>();
    extra->m_pScene = extra->m_Importer.ReadFileFromMemory(
        rawBuffer, rawSize,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals
    );
    if (!extra->m_pScene ||
            (extra->m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
            !extra->m_pScene->mRootNode) {
        printf("AssimpMeshResourceLoader VLoadResource failed\n");
        return false;
    }
    handle->SetExtra(extra);
    return true;
}
