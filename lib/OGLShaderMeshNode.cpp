#include "OGLShaderMeshNode.h"
#include "OGLSdkMeshResourceExtraData.h"
#include "OGLRenderer.h"
#include "Resource.h"
#include "ResHandle.h"
#include "ResCache.h"
#include "Scene.h"

OGLShaderMeshNode::OGLShaderMeshNode(
        const ActorId actorId,
        std::string name,
        std::string sdkMeshFileName,
        RenderPass renderPass,
        const Color& color,
        const Mat4x4* t) :
    SceneNode(actorId, name, renderPass, color, t)
{
    m_SdkMeshFileName = sdkMeshFileName;
}
OGLShaderMeshNode::~OGLShaderMeshNode()
{}

bool OGLShaderMeshNode::VOnRestore(Scene* pScene)
{
    SceneNode::VOnRestore(pScene);

    // Load mesh
    Resource resource(m_SdkMeshFileName);
    std::shared_ptr<ResHandle> pResourceHandle =
        g_ResCache->GetHandle(&resource);
    std::shared_ptr<OGLSdkMeshResourceExtraData> extra =
        std::static_pointer_cast<OGLSdkMeshResourceExtraData>(pResourceHandle->GetExtra());

    // Create vertex buffer
    Sdkmesh& mesh = extra->GetMesh();
    const std::vector<std::vector<SdkmeshMath::PosNormalTexTan>>& verts = mesh.GetSdkmeshVertexBuffer();
    printf("sdkmesh verts size: %lu\n", (long unsigned int)verts.size());

    // Shader
    const char* vertStr =
        "#version 330 core                              \n"
        "                                               \n"
        "layout (location = 0) in vec3 aPos;            \n"
        "layout (location = 1) in vec3 aColor;          \n"
        "                                               \n"
        "out vec3 fragColor;                            \n"
        "                                               \n"
        "uniform mat4 uModelMat;                        \n"
        "uniform mat4 uViewMat;                         \n"
        "uniform mat4 uProjMat;                         \n"
        "                                               \n"
        "void main()                                    \n"
        "{                                              \n"
        "   gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPos,1.0);     \n"
        "   fragColor = aColor;                                                 \n"
        "}                                                                      \n";
    const char* fragStr =
        "#version 330 core                      \n"
        "                                       \n"
        "out vec4 FragColor;                    \n"
        "in vec3 fragColor;                     \n"
        "                                       \n"
        "void main()                            \n"
        "{                                      \n"
        "   FragColor = vec4(fragColor,1.0);    \n"
        "}                                      \n";
    if (!m_Shader.LoadFromString(vertStr, fragStr)) {
        throw std::runtime_error("OGLShaderMeshNode shader load from string failed\n");
    }

    return true;
}

bool OGLShaderMeshNode::VRender(Scene* pScene)
{
    //TODO
    //OGLRenderer* rndr = static_cast<OGLRenderer*>(pScene->GetRenderer());
    //rndr->SetShader(m_Shader);
    return true;
}