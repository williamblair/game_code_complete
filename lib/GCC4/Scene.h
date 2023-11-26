#ifndef GCC4_SCENE_H_INCLUDED
#define GCC4_SCENE_H_INCLUDED

#include <map>
#include <memory>
#include <stack>

#include "ISceneNode.h"
#include "IRenderer.h"
#include "ComponentTypes.h"
#include "MatrixStack.h"
#include "AlphaSceneNode.h"

typedef std::map<ActorId,std::shared_ptr<ISceneNode>> SceneActorMap;

// forward declarations
class SceneNode;
class CameraNode;
class SkyNode;
class LightNode;
class LightManager;

class Scene
{
public:
    Scene(std::shared_ptr<IRenderer> pRenderer);
    virtual ~Scene();

    bool OnRender();
    bool OnRestore();
    bool OnLostDevice();
    bool OnUpdate(const int deltaMilliseconds);

    std::shared_ptr<ISceneNode> FindActor(ActorId id);
    bool AddChild(ActorId id, std::shared_ptr<ISceneNode> kid);
    bool RemoveChild(ActorId id);

    void SetCamera(std::shared_ptr<CameraNode> camera) { m_Camera = camera; }
    const std::shared_ptr<CameraNode> GetCamera() const { return m_Camera; }

    void PushAndSetMatrix(const Mat4x4& toWorld);
    void PopMatrix();
    const Mat4x4* GetTopMatrix() { return m_MatrixStack->GetTop(); }

    LightManager* GetLightManager() { return m_LightManager; }
    IRenderer* GetRenderer() { return m_Renderer.get(); }

    void AddAlphaSceneNode(AlphaSceneNode* asn) { m_AlphaSceneNodes.push_back(asn); }

    void RenerAlphaPass();

protected:
    std::shared_ptr<SceneNode> m_Root;
    std::shared_ptr<CameraNode> m_Camera;
    std::shared_ptr<IRenderer> m_Renderer;

    MatrixStack* m_MatrixStack;
    AlphaSceneNodes m_AlphaSceneNodes;
    SceneActorMap m_ActorMap;
    LightManager* m_LightManager;

    void RenderAlphaPass();
};

#endif // GCC4_SCENE_H_INCLUDED
