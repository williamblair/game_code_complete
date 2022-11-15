#ifndef OGL_SKYNODE_H_INCLUDED
#define OGL_SKYNODE_H_INCLUDED

#include <string>

#include "SceneNode.h"
#include "OGLShader.h"
#include "OGLVertexBuffer.h"
#include "CameraNode.h"

class OGLSkyNode : public SceneNode
{
public:
    OGLSkyNode(const char* textureFile, std::shared_ptr<CameraNode> camera);
    virtual ~OGLSkyNode();
    bool VOnRestore(Scene* pScene);
    bool VRender(Scene* pScene);
    bool VPreRender(Scene* pScene);
    bool VIsVisible(Scene* pScene) const { return m_bActive; }
private:
    std::string m_TextureBaseName;
    std::shared_ptr<CameraNode> m_Camera;
    bool m_bActive;
    unsigned int m_CubeTexId;
    OGLVertexBuffer m_VertBuf;
    OGLShader m_Shader;
};

#endif // OGL_SKYNODE_H_INCLUDED
