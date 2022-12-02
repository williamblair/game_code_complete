#include <stdexcept>

#include "AssimpMeshNode.h"
#include "AssimpMeshResourceExtraData.h"
#include "OGLTextureResourceExtraData.h"
#include "OGLCreateTexture.h"
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
    m_TexId = 0;
}
AssimpMeshNode::~AssimpMeshNode()
{
    if (m_TexId) { glDeleteTextures(1, &m_TexId); }
}

bool AssimpMeshNode::VOnRestore(Scene* pScene)
{
    SceneNode::VOnRestore(pScene);

    {
        // Load mesh/vertex buffer
        Resource resource(m_MeshFileName);
        std::shared_ptr<ResHandle> pResourceHandle =
            g_ResCache->GetHandle(&resource);
        std::shared_ptr<AssimpMeshResourceExtraData> extra =
            std::static_pointer_cast<AssimpMeshResourceExtraData>(pResourceHandle->GetExtra());
        if (!extra) {
            printf("AssimpMeshNode on restore get extra is null\n");
            return false;
        }
        if (!ProcessNode(extra->GetScene()->mRootNode, extra->GetScene())) {
            return false;
        }
    }
    {
        // Load texture - TESTING
        Resource resource("teapot.png");
        std::shared_ptr<ResHandle> pResHandle = g_ResCache->GetHandle(&resource);
        auto pExtra = std::static_pointer_cast<OGLTextureResourceExtraData>(pResHandle->GetExtra());
        if (!pExtra) {
            printf("AssimpMeshNode load texture extra is null\n");
            return false;
        }
        m_TexId = OGLCreateTexture(
            pExtra->GetTexture()->GetData(),
            pExtra->GetTexture()->GetWidth(), pExtra->GetTexture()->GetHeight(),
            pExtra->GetTexture()->GetBytesPerPixel()
        );
    }

    // Shader
    const char* vertStr =
        "#version 330 core                              \n"
        "                                               \n"
        "layout (location = 0) in vec3 aPos;            \n"
        "layout (location = 1) in vec3 aNorm;           \n"
        "layout (location = 2) in vec2 aTexCoord;       \n"
        "                                               \n"
        "out vec2 fragTexCoord;                         \n"
        "out vec3 fragNormal;                           \n"
        "                                               \n"
        "uniform mat4 uModelMat;                        \n"
        "uniform mat4 uViewMat;                         \n"
        "uniform mat4 uProjMat;                         \n"
        "                                               \n"
        "void main()                                    \n"
        "{                                              \n"
        "   gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPos,1.0);     \n"
        "   fragTexCoord = aTexCoord;                                           \n"
        "                                                                       \n"
        "   mat4 normalMat = transpose(inverse(uModelMat));                     \n"
        "   fragNormal = normalize(vec3(normalMat*vec4(aNorm,1.0)));            \n"
        "}                                                                      \n";
    const char* fragStr =
        "#version 330 core                      \n"
        "                                       \n"
        "out vec4 FragColor;                    \n"
        "                                       \n"
        "in vec3 fragNormal;                    \n"
        "in vec2 fragTexCoord;                  \n"
        "                                       \n"
        "uniform sampler2D uDiffuseTex;         \n"
        "                                       \n"
        "uniform vec3 uLightDiffuse[8];                 \n"
        "uniform vec3 uLightDir[8];                     \n"
        "uniform vec3 uAmbient;                         \n"
        "uniform int uNumLights;                        \n"
        "                                               \n"
        "uniform vec4 uDiffuseObjectColor;              \n"
        "uniform vec4 uAmbientObjectColor;              \n"
        "                                               \n"
        "vec3 calcAmbLight()                            \n"
        "{                                              \n"
        "   return uAmbient *                           \n"
        "       uAmbientObjectColor.rgb;                \n"
        "}                                              \n"
        "                                                       \n"
        "vec3 calcDirLight(int i)                               \n"
        "{                                                      \n"
        "   vec3 norm = normalize(fragNormal);                  \n"
        "   vec3 lightDir = normalize(uLightDir[i]);            \n"
        "   float nDotL = max(dot(lightDir,norm),0);            \n"
        "   vec3 diffuse = uLightDiffuse[i] *                   \n"
        "        uDiffuseObjectColor.rgb *                      \n"
        "        nDotL;                                         \n"
        "   return diffuse;                                     \n"
        "}                                                      \n"
        "                                                       \n"
        "void main()                                            \n"
        "{                                                      \n"
        "   FragColor = vec4(calcAmbLight(),0.0);               \n"
        "   for (int i=0; i<uNumLights; ++i) {                  \n"
        "       FragColor += vec4(calcDirLight(i),0.0);         \n"
        "   }                                                   \n"
        "   FragColor.rgb =                                     \n"
        "        texture(uDiffuseTex,fragTexCoord).rgb *        \n"
        "        FragColor.rgb;                                 \n"
        "   FragColor.a = texture(uDiffuseTex,fragTexCoord).a;  \n"
        "}                                                      \n";
    if (!m_Shader.LoadFromString(vertStr, fragStr)) {
        throw std::runtime_error("OGLShaderMeshNode shader load from string failed\n");
    }

    return true;
}

bool AssimpMeshNode::VRender(Scene* pScene)
{
    SceneNode::VRender(pScene);
    int lightCount = pScene->GetLightManager()->GetLightCount(this);
    OGLRenderer* rndr = static_cast<OGLRenderer*>(pScene->GetRenderer());
    rndr->SetShader(m_Shader);
    m_Shader.SetInt("uNumLights", lightCount);
    if (lightCount > 0) {
        char uniformName[128];
        const Vec4* lightAmbient = pScene->GetLightManager()->GetLightAmbient(this);
        const Color* lightCols = pScene->GetLightManager()->GetLightDiffuse(this);
        const Vec4* lightDirs = pScene->GetLightManager()->GetLightDirection(this);
        m_Shader.SetVec3("uAmbient", Vec3(lightAmbient->x, lightAmbient->y, lightAmbient->z));
        for (int i=0; i<lightCount; ++i) {
            sprintf(uniformName, "uLightDiffuse[%d]", i);
            m_Shader.SetVec3(uniformName, Vec3(lightCols[i].r,lightCols[i].g,lightCols[i].b));
            sprintf(uniformName, "uLightDir[%d]", i);
            m_Shader.SetVec3(uniformName, Vec3(lightDirs[i].x,lightDirs[i].y,lightDirs[i].z));
        }
    }
    // TESTING //////////////////////////////////////////////////////////
    //m_Shader.SetInt("uNumLights", 1);
    //m_Shader.SetVec3("uLightDiffuse[0]", Vec3(1.0f,1.0f,1.0f));
    //m_Shader.SetVec3("uLightDir[0]", Vec3(0.0f,1.0f,0.0f));
    //m_Shader.SetVec3("uAmbient", Vec3(0.1f,0.1f,0.1f));
    m_Shader.SetVec4("uDiffuseObjectColor", Vec4(1.0f,1.0f,1.0f,1.0f));
    m_Shader.SetVec4("uAmbientObjectColor", Vec4(0.1f,0.1f,0.1f,1.0f));
    glBindTexture(GL_TEXTURE_2D, m_TexId);
    ////////////////////////////////////////////////////////////////////
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
        std::vector<OGLVertexBuffer::VertexTextured> verts(pMesh->mNumVertices);
        std::vector<uint32_t> indices;
        for (size_t j=0; j<pMesh->mNumVertices; ++j)
        {
            OGLVertexBuffer::VertexTextured& vert = verts[j];
            vert.x = pMesh->mVertices[j].x;
            vert.y = pMesh->mVertices[j].y;
            vert.z = pMesh->mVertices[j].z;
            if (pMesh->HasNormals()) {
                vert.nx = pMesh->mNormals[j].x;
                vert.ny = pMesh->mNormals[j].y;
                vert.nz = pMesh->mNormals[j].z;
            }
            //vert.r = 1.0f;
            //vert.g = 0;
            //vert.b = 0;
            if (pMesh->mTextureCoords[0]) {
                vert.u = pMesh->mTextureCoords[0][j].x;
                vert.v = pMesh->mTextureCoords[0][j].y;
            }
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
            OGLVertexBuffer::POS_TEXCOORD,
            verts.data(), verts.size(),
            indices.size() > 0 ? indices.data() : nullptr, indices.size()))
        {
            printf("Assimp mesh vert buf init failed\n");
            return false;
        }
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

