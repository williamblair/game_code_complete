#include "Scene.h"
#include "SceneNode.h"
#include "GCCTime.h"
#include "RootNode.h"
#include "CameraNode.h"
#include "LightManager.h"

Scene::Scene()
{
    m_Root.reset(new RootNode());
    m_LightManager = new LightManager();
    m_MatrixStack = new MatrixStack();
}

Scene::~Scene()
{
    delete m_MatrixStack;
    delete m_LightManager;
}

bool Scene::OnRender()
{
    if (m_Root && m_Camera)
    {
        // The scene root could be anything, but it's usually a SceneNode
        // with the identity matrix
        m_Camera->SetViewTransform(this);
        m_LightManager->CalcLighting(this);

        if (m_Root->VPreRender(this) == true)
        {
            m_Root->VRender(this);
            m_Root->VRenderChildren(this);
            m_Root->VPostRender(this);
        }
    }

    RenderAlphaPass();

    return true;
}

bool Scene::OnRestore()
{
    if (!m_Root) { return true; }

    return m_Root->VOnRestore(this);
}

bool Scene::OnLostDevice()
{
    //TODO
    return true;
}

bool Scene::OnUpdate(const int deltaMilliseconds)
{
    (void)deltaMilliseconds;
    if (!m_Root) { return true; }
    static uint32_t lastTime = timeGetTime();
    uint32_t elapsedTime = 0;
    uint32_t now = timeGetTime();
    elapsedTime = now - lastTime;
    lastTime = now;

    return m_Root->VOnUpdate(this, elapsedTime);
}

std::shared_ptr<ISceneNode> Scene::FindActor(ActorId id)
{
    SceneActorMap::iterator i = m_ActorMap.find(id);
    if (i == m_ActorMap.end()) {
        return std::shared_ptr<ISceneNode>();
    }
    return i->second;
}

bool Scene::AddChild(ActorId id, std::shared_ptr<ISceneNode> kid)
{
    if (id == INVALID_ACTOR_ID) { return false; }
    m_ActorMap[id] = kid;
    std::shared_ptr<LightNode> pLight = std::dynamic_pointer_cast<LightNode>(kid);
    if (pLight != nullptr &&
            m_LightManager->m_Lights.size()+1 < MAXIMUM_LIGHTS_SUPPORTED)
    {
        m_LightManager->m_Lights.push_back(pLight);
    }
    return m_Root->VAddChild(kid);
}
bool Scene::RemoveChild(ActorId id)
{
    if (id == INVALID_ACTOR_ID) { return false; }
    std::shared_ptr<ISceneNode> kid = FindActor(id);
    std::shared_ptr<LightNode> pLight = std::dynamic_pointer_cast<LightNode>(kid);
    if (pLight != nullptr) {
        m_LightManager->m_Lights.remove(pLight);
    }
    m_ActorMap.erase(id);
    return m_Root->VRemoveChild(id);
}

void Scene::PushAndSetMatrix(const Mat4x4& toWorld)
{
    m_MatrixStack->Push();
    m_MatrixStack->MultMatrixLocal(&toWorld);
    //TODO
    //SetTransform(D3DTS_WORLD, m_MatrixStack->GetTop());
}

void Scene::PopMatrix()
{
    m_MatrixStack->Pop();
    //TODO
    //SetTransform(D3DTS_WORLD, m_MatrixStack->GetTop());
}

void Scene::RenderAlphaPass()
{
    // D3DRendererAlphaPass11 alphaPass;
    m_AlphaSceneNodes.sort();
    while (!m_AlphaSceneNodes.empty())
    {
        AlphaSceneNodes::reverse_iterator i = m_AlphaSceneNodes.rbegin();
        //TODO
        //DXUTGetD3DDevice()->SetTransform(D3DTS_WORLD, &((*i)->m_Concat))
        (*i)->m_pNode->VRender(this);
        delete (*i);
        m_AlphaSceneNodes.pop_back();
    }
}

