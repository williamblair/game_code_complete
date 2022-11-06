#include <stdexcept>
#include "OGLSkyNode.h"
#include "stb_image.h"

// https://learnopengl.com/Advanced-OpenGL/Cubemaps

OGLSkyNode::OGLSkyNode(const char* textureFile, std::shared_ptr<CameraNode> camera) :
    SceneNode(INVALID_ACTOR_ID, "Sky", RenderPass_Sky, g_White, &Mat4x4::g_Identity) ,
    m_TextureBaseName(textureFile),
    m_Camera(camera),
    m_bActive(true),
    m_CubeTexId(0)
{
    // Load the vertex buffer
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    if (!m_VertBuf.Init(OGLVertexBuffer::POS, skyboxVertices, 6*6, nullptr, 0)) {
        throw std::runtime_error("Failed to init sky node vertex buffer");
    }

    // Load the texture
    glGenTextures(1, &m_CubeTexId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTexId);
    int width, height, bytesPerPixel;
    std::string faces[6] = {
        m_TextureBaseName + "_r.png", // right
        m_TextureBaseName + "_l.png", // left
        m_TextureBaseName + "_t.png", // top
        m_TextureBaseName + "_b.png", // bottom
        m_TextureBaseName + "_f.png", // front
        m_TextureBaseName + "_b.png" // back
    };
    for (int i=0; i<6; ++i) {
        uint8_t* data = stbi_load(faces[i].c_str(), &width,&height,&bytesPerPixel, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            throw std::runtime_error("Failed to load sky box texture: " + faces[i]);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Load the shader
    const char* vertexShaderSrc = 
        "#version 330 core                                          \n"
        "layout (location = 0) in vec3 aPos;                        \n"
        "                                                           \n"
        "out vec3 TexCoords;                                        \n"
        "                                                           \n"
        "uniform mat4 uProjMat;                                     \n"
        "uniform mat4 uViewMat;                                     \n"
        "                                                           \n"
        "void main()                                                \n"
        "{                                                          \n"
        "   TexCoords = aPos;                                       \n"
        "   gl_Position = uProjMat * uViewMat * vec4(aPos,1.0);     \n"
        "}                                                          \n";
    const char* fragmentShaderSrc = 
        "#version 330 core                                          \n"
        "out vec4 FragColor;                                        \n"
        "                                                           \n"
        "in vec3 TexCoords;                                         \n"
        "                                                           \n"
        "uniform samplerCube uSkybox                                \n"
        "                                                           \n"
        "void main()                                                \n"
        "{                                                          \n"
        "   FragColor = texture(uSkybox, TexCoords);                \n"
        "}                                                          \n";
    if (!m_Shader.LoadFromString(vertexShaderSrc,fragmentShaderSrc)) {
        throw std::runtime_error("Sky Node shader init failed");
    }
}
OGLSkyNode::~OGLSkyNode()
{
}

bool OGLSkyNode::VOnRestore(Scene* pScene)
{
    //TODO
    return true;
}

bool OGLSkyNode::VRender(Scene* pScene)
{
    //TODO
    return true;
}

bool OGLSkyNode::VPreRender(Scene* pScene)
{
    //TODO
    return true;
}

