#include "AssimpMeshNode.h"
#include "AssimpMeshResourceExtraData.h"
#include "OGLRenderer.h"
#include "Resource.h"
#include "ResHandle.h"
#include "ResCache.h"
#include "Scene.h"

AssimpMeshNode::AssimpMeshNode(
        const ActorId actorId,
        std::string name,
        std::string meshFileName,
        RenderPass renderPass,
        const Color& color,
        const Mat4x4* t) :
    SceneNode(actorId, name, renderPass, color, t)
{
    m_MeshFileName = meshFileName;
}
AssimpMeshNode::~AssimpMeshNode()
{}

bool AssimpMeshNode::VOnRestore(Scene* pScene)
{
    SceneNode::VOnRestore(pScene);

    // Load mesh/vertex buffer
    Resource resource(m_MeshFileName);
    std::shared_ptr<ResHandle> pResourceHandle =
        g_ResCache->GetHandle(&resource);
    std::shared_ptr<AssimpMeshResourceExtraData> extra =
        std::static_pointer_cast<AssimpMeshResourceExtraData>(pResourceHandle->GetExtra());
    if (!extra) {
        printf("AssimpMeshNode on restore get extra is null\n");
    }
    if (!ProcessNode(extra->GetScene()->mRootNode, extra->GetScene())) {
        return false;
    }

    // Shader
    const char* vertStr =
        "#version 330 core                              \n"
        "                                               \n"
        "layout (location = 0) in vec3 aPos;            \n"
        "layout (location = 1) in vec3 aNorm;           \n"
        "layout (location = 2) in vec3 aColor;          \n"
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

bool AssimpMeshNode::VRender(Scene* pScene)
{
    SceneNode::VRender(pScene);

    OGLRenderer* rndr = static_cast<OGLRenderer*>(pScene->GetRenderer());
    rndr->SetShader(m_Shader);
    //Mat4x4 testProj = Perspective(Deg2Rad(60.0f), 640.0f/480.0f, 0.1f, 1000.0f);
    //Mat4x4 testView = Mat4x4::g_Identity;
    //Mat4x4 testModel = Translate(0.0f,0.0f,-5.0f) * Scale(0.01f,0.01f,0.01f);
    //m_Shader.Use();
    //m_Shader.SetMat4("uModelMat", testModel);
    //m_Shader.SetMat4("uViewMat", testView);
    //m_Shader.SetMat4("uProjMat", testProj);
    //Mat4x4 testMvp = testProj * testView * testModel;
    //Vec3 testVec(m_TestVertColored.x,m_TestVertColored.y,m_TestVertColored.z);
    //Vec3 testTform = Transform(testMvp, testVec);
    //if (!m_Shader.SetMat4("uMvpMat", testMvp)) {
    //    throw std::runtime_error("Failed to set mvp mat");
    //}
    for (std::shared_ptr<OGLVertexBuffer>& vb : m_VertBufs) {
        rndr->DrawVertexBuffer(*vb);
    }
    return true;
}

bool AssimpMeshNode::ProcessNode(aiNode* pNode, const aiScene* pScene)
{
    // process each mesh located at the current node
    for (size_t i=0; i<pNode->mNumMeshes; ++i)
    {
        aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
        std::vector<OGLVertexBuffer::VertexColored> verts(pMesh->mNumVertices);
        std::vector<uint32_t> indices;
        // TODO - textures
        for (size_t j=0; j<pMesh->mNumVertices; ++j)
        {
            OGLVertexBuffer::VertexColored& vert = verts[j];
            vert.x = pMesh->mVertices[j].x;
            vert.y = pMesh->mVertices[j].y;
            vert.z = pMesh->mVertices[j].z;
            if (pMesh->HasNormals()) {
                vert.nx = pMesh->mNormals[j].x;
                vert.ny = pMesh->mNormals[j].y;
                vert.nz = pMesh->mNormals[j].z;
            }
            vert.r = 1.0f;
            vert.g = 0;
            vert.b = 0;
            // TODO
            // if (pMesh->mTextureCoords[0]) {
            // vert.u = pMesh->mTextureCoords[0][j].x;
            // vert.v = pMesh->mTextureCoords[0][j].y;
            //}
            // TODO - tangent/bitangent
        }
        for (size_t j=0; j<pMesh->mNumFaces; ++j)
        {
            aiFace face = pMesh->mFaces[j];
            for (size_t k=0; k<face.mNumIndices; ++k) {
                indices.push_back(face.mIndices[k]);
            }
        }
        m_VertBufs.push_back(std::make_shared<OGLVertexBuffer>());
        OGLVertexBuffer& vb = *m_VertBufs[m_VertBufs.size()-1];
        if (!vb.Init(
            OGLVertexBuffer::POS_COLOR,
            verts.data(), verts.size(),
            indices.size() > 0 ? indices.data() : nullptr, indices.size()))
        {
            printf("Assimp mesh vert buf init failed\n");
            return false;
        }
        m_TestVertColored = verts[0];
    }

    // after we've processed all of the meshes (if any),
    // we then recursively process each of the children nodes
    for (size_t i=0; i<pNode->mNumChildren; ++i)
    {
        if (!ProcessNode(pNode->mChildren[i], pScene)) {
            return false;
        }
    }

    return true;
}

