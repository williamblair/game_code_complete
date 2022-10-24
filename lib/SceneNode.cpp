#include "SceneNode.h"
#include "Scene.h"
#include "AlphaSceneNode.h"

SceneNode::SceneNode(
        ActorId actorId,
        std::string name,
        RenderPass renderPass,
        const Color& diffuseColor,
        const Mat4x4* to,
        const Mat4x4* from
)
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
    
void SceneNode::VSetTransform(const Mat4x4* toWorld, const Mat4x4* fromWorld)
{
    m_Props.m_ToWorld = *toWorld;
    if (!fromWorld) {
        m_Props.m_FromWorld = Inverse(m_Props.m_ToWorld);
    } else {
        m_Props.m_FromWorld = *fromWorld;
    }
}

bool SceneNode::VOnRestore(Scene* pScene)
{
    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (i != end)
    {
        (*i)->VOnRestore(pScene);
        ++i;
    }
    return true;
}

bool SceneNode::VOnUpdate(Scene* pScene, uint32_t const elapsedMs)
{
    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (i != end)
    {
        (*i)->VOnUpdate(pScene, elapsedMs);
    }
    return true;
}

bool SceneNode::VPreRender(Scene* pScene)
{
    pScene->PushAndSetMatrix(m_Props.m_ToWorld);
    return true;
}

bool SceneNode::VPostRender(Scene* pScene)
{
    pScene->PopMatrix();
    return true;
}

bool SceneNode::VIsVisible(Scene* pScene) const
{
    // transform the location of this node into the camera space
    // of the camera attached to the scene
    Mat4x4 toWorld, fromWorld;
    pScene->GetCamera()->VGet()->Transform(&toWorld, &fromWorld);
    Vec3 pos = VGet()->GetToWorld().GetPosition();
    pos = Transform(fromWorld, pos);
    Frustum const& frustum = pScene->GetCamera()->GetFrustum();
    return frustum.Inside(pos, VGet()->Radius());
}

bool SceneNode::VRenderChildren(Scene* pScene)
{
    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (i != end)
    {
        if ((*i)->VPreRender(pScene))
        {
            // you could short circuit rendering if an object fails from VPreRender

            // don't render this node if you can't see it
            if ((*i)->VIsVisible(pScene))
            {
                float alpha = (*i)->VGet()->m_Material.GetAlpha();
                if (alpha >= 0.99f) {
                    (*i)->VRender(pScene);
                } else if (alpha >= 0.01f) {
                    // The object isn't totally transparent
                    AlphaSceneNode* asn = new AlphaSceneNode;
                    assert(asn);
                    asn->m_pNode = *i;
                    asn->m_Concat = *pScene->GetTopMatrix();

                    Vec3 worldPos(asn->m_Concat.GetPosition());
                    Mat4x4 fromWorld = pScene->GetCamera()->VGet()->FromWorld();
                    Vec4 screenPos = Transform(fromWorld, worldPos);
                    asn->m_ScreenZ = screenPos.z;
                    pScene->AddAlphaSceneNode(asn);
                }
            }

            (*i)->VRenderChildren(pScene);
        }

        (*i)->VPostRender(pScene);
        ++i;
    }
    return true;
}

bool SceneNode::VAddChild(std::shared_ptr<ISceneNode> kid)
{
    m_Children.push_back(kid);

    // the radius of the sphere should be fixed right here
    Vec3 kidPos = kid->VGet()->GetToWorld().GetPosition();
    Vec3 dir = kidPos - m_Props.GetToWorld().GetPosition();

    float newRadius = Mag(dir) + kid->VGet()->Radius();
    if (newRadius > m_Props.m_Radius) {
        m_Props.m_Radius = newRadius;
    }
    return true;
}
