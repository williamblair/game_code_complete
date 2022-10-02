#ifndef GCCSCENE_H_INCLUDED
#define GCCSCENE_H_INCLUDED

#include <map>
#include <memory>

#include <ISceneNode.h>
#include <IRenderer.h>
#include <ComponentTypes.h>
#include <MatrixStack.h>
#include <AlphaSceneNode.h>

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
    Scene();
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
    const Mat4x4* GetTopMatrix();

    LightManager* GetLightManager() { return m_LightManager; }

    void AddAlphaSceneNode(AlphaSceneNode* asn) { m_AlphaSceneNodes.push_back(asn); }

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

#endif // GCCSCENE_H_INCLUDED
