#include "RootNode.h"
#include "Scene.h"
#include <cstdio>

RootNode::RootNode() :
    SceneNode(
        /*optional_empty()*/INVALID_ACTOR_ID, // TODO
        "Root",
        RenderPass_0,
        g_White,
        &Mat4x4::g_Identity)
{
    m_Children.reserve(RenderPass_Last);
    
    std::shared_ptr<SceneNode> staticGroup = std::make_shared<SceneNode>(
        INVALID_ACTOR_ID,
        "StaticGroup",
        RenderPass_Static,
        g_White,
        &Mat4x4::g_Identity
    );
    m_Children.push_back(staticGroup); // RenderPass_Static = 0

    std::shared_ptr<SceneNode> actorGroup = std::make_shared<SceneNode>(
        INVALID_ACTOR_ID,
        "ActorGroup",
        RenderPass_Actor,
        g_White,
        &Mat4x4::g_Identity
    );
    m_Children.push_back(actorGroup); // RenderPass_Actor = 1

    std::shared_ptr<SceneNode> skyGroup = std::make_shared<SceneNode>(
        INVALID_ACTOR_ID,
        "SkyGroup",
        RenderPass_Sky,
        g_White,
        &Mat4x4::g_Identity
    );
    m_Children.push_back(skyGroup); // RenderPass_Sky = 2

    std::shared_ptr<SceneNode> invisibleGroup = std::make_shared<SceneNode>(
        INVALID_ACTOR_ID,
        "InvisibleGroup",
        RenderPass_NotRendered,
        g_White,
        &Mat4x4::g_Identity
    );
    m_Children.push_back(invisibleGroup); // RenderPass_NotRendered = 3
}

bool RootNode::VAddChild(std::shared_ptr<ISceneNode> kid)
{
    // Children that divide the scene graph into render passes.
    // Scene nodes will get added to these children based on the value of the
    // render pass member variable
    RenderPass pass = kid->VGet()->GetRenderPass();
    if ((unsigned)pass >= m_Children.size() || !m_Children[pass])
    {
        printf("There is no such render pass\n");
        return false;
    }

    return m_Children[pass]->VAddChild(kid);
}

bool RootNode::VRenderChildren(Scene* pScene)
{
    for (int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
    {
        switch (pass)
        {
        case RenderPass_Static:
        case RenderPass_Actor:
            m_Children[pass]->VRenderChildren(pScene);
            break;

        case RenderPass_Sky:
        {
            std::shared_ptr<IRenderState> skyPass = pScene->GetRenderer()->VPrepareSkyboxPass();
            m_Children[pass]->VRenderChildren(pScene);
            break;
        }
        default:
            break;
        }
    }

    return true;
}
