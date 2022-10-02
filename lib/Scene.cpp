#include <Scene.h>
#include <SceneNode.h>

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
    //TODO
    return true;
}

bool Scene::OnRestore()
{
    //TODO
    return true;
}

bool Scene::OnLostDevice()
{
    //TODO
    return true;
}

bool Scene::Update(const int deltaMilliseconds)
{
    //TODO
    return true;
}

std::shared_ptr<ISceneNode> Scene::FindActor(ActorId id)
{
    //TODO
    return std::shared_ptr<ISceneNode>();
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
    //TODO
}

void Scene::PopMatrix()
{
    //TODO
}

