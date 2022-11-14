#ifndef GCC4_ASSIMP_MESH_NODE_H_INCLUDED
#define GCC4_ASSIMP_MESH_NODE_H_INCLUDED

#include <assimp/scene.h>

#include "SceneNode.h"
#include "OGLShader.h"
#include "OGLVertexBuffer.h"

class AssimpMeshNode : public SceneNode
{
public:
    AssimpMeshNode(
        const ActorId actorId,
        std::string name,
        std::string meshFileName,
        RenderPass renderPass,
        const Color& color,
        const Mat4x4* t
    );
    virtual ~AssimpMeshNode();

    virtual bool VOnRestore(Scene* pScene);
    virtual bool VOnLostDevice(Scene* pScene) { return true; }
    virtual bool VRender(Scene* pScene);
private:
    std::string m_MeshFileName;
    OGLShader m_Shader;
    std::vector<std::shared_ptr<OGLVertexBuffer>> m_VertBufs;
    unsigned int m_TexId;

    bool ProcessNode(aiNode* pNode, const aiScene* pScene);
};

#endif // GCC4_ASSIMP_MESH_NODE_H_INCLUDED
