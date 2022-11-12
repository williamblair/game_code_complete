#ifndef GCC4_OGL_SHADER_MESH_NODE_H_INCLUDED
#define GCC4_OGL_SHADER_MESH_NODE_H_INCLUDED

#include "SceneNode.h"
#include "OGLShader.h"
#include "OGLVertexBuffer.h"

class OGLShaderMeshNode : public SceneNode
{
public:
    OGLShaderMeshNode(
        const ActorId actorId,
        std::string name,
        std::string sdkMeshFileName,
        RenderPass renderPass,
        const Color& color,
        const Mat4x4* t
    );
    virtual ~OGLShaderMeshNode();

    virtual bool VOnRestore(Scene* pScene);
    virtual bool VOnLostDevice(Scene* pScene) { return true; }
    virtual bool VRender(Scene* pScene);
private:
    std::string m_SdkMeshFileName;
    OGLShader m_Shader;
    OGLVertexBuffer m_VertBuf;
};

#endif // GCC4_OGL_SHADER_MESH_NODE_H_INCLUDED
