#include <memory>
#include <vector>

#include <ISceneNode.h>
#include <SceneNodeProperties.h>

typedef std::vector<std::shared_ptr<ISceneNode>> SceneNodeList;

class SceneNode : public ISceneNode
{

friend class Scene;

public:

    SceneNode(
            ActorId actorId,
            std::string name,
            RenderPass renderPass,
            const Color& diffuseColor,
            const Mat4x4* to,
            const Mat4x4* from = nullptr)
    {
        m_pParent = nullptr;
        m_Props.m_ActorId = actorId;
        m_Props.m_Name = name;
        m_Props.m_RenderPass = renderPass;
        m_Props.m_AlphaType = AlphaOpaque;
        VSetTransform(to, from);
        SetRadius(0);
        m_Props.m_Material.SetDiffuse(diffuseColor);
    }
    
    virtual ~SceneNode();
    virtual const SceneNodeProperties* const VGet() const { return &m_Props; }
    virtual void VSetTransform(
        const Mat4x4* toWorld,
        const Mat4x4* fromWorld = nullptr
    );
    
    virtual bool VOnRestore(Scene* pScene);
    virtual bool VOnUpdate(Scene* pScene, uint32_t const elapsedMs);
    virtual bool VPreRender(Scene* pScene);
    virtual bool VIsVisible(Scene* pScene) const;
    virtual bool VRender(Scene* pScene) { return true; }
    virtual bool VRenderChildren(Scene* pScene);
    virtual bool VPostRender(Scene* pScene);
    
    virtual bool VAddChild(std::shared_ptr<ISceneNode> kid);
    virtual bool VRemoveChild(ActorId id);
    
    void SetAlpha(float alpha) { m_Props.SetAlpha(alpha); }
    float GetAlpha() const { return m_Props.Alpha(); }
    
    Vec3 GetPosition() const { return m_Props.m_ToWorld.GetPosition(); }
    void SetPosition(const Vec3& pos) { m_Props.m_ToWorld.SetPosition(pos); }
    Vec3 GetDirection() const {
        return m_Props.m_ToWorld.GetDirection();
    }
    
    void SetRadius(const float radius) { m_Props.m_Radius = radius; }
    void SetMaterial(const Material& mat) { m_Props.m_Material = mat; }

protected:

    SceneNodeList m_Children;
    SceneNode* m_pParent;
    SceneNodeProperties m_Props;

};