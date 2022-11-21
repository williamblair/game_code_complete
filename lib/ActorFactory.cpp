#include <iostream>
#include "ActorFactory.h"
#include "MakeStrongPtr.h"
//#include "AmmoPickup.h"
//#include "HealthPickup.h"

ActorFactory::ActorFactory()
{
    //m_actorComponentCreators["AmmoPickup"] = AmmoPickup::Create;
    //m_actorComponentCreators["HealthPickup"] = HealthPickup::Create;

    m_LastActorId = INVALID_ACTOR_ID;

    //TODO
    //m_ComponentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::g_Name));
    // ... other component types
}

ActorFactory::~ActorFactory()
{
}

StrongActorPtr ActorFactory::CreateActor(
    const char* actorResourceXML,
    XMLElement* overrides,
    const Mat4x4* initialTransform,
    const ActorId serversActorId)
{
    // Grab the root XML node
    XMLElement* pRoot = 
        XmlResourceLoader::LoadAndReturnRootXmlElement(actorResourceXML);
    if (!pRoot) {
        std::cout << __func__ << ": Failed to create actor from resource: " 
                  << actorResourceXML << std::endl;
        return StrongActorPtr();
    }

    // Create the actor instance
    ActorId nextActorId = serversActorId;
    if (nextActorId == INVALID_ACTOR_ID) {
        nextActorId = GetNextActorId();
    }
    StrongActorPtr pActor(new Actor(nextActorId));
    if (!pActor->Init(pRoot)) {
        std::cout << __func__ << ": failed to init actor "
                  << actorResourceXML << std::endl;
        return StrongActorPtr();
    }

    bool bInitialTransformSet = false;

    // loop through each child XML element and load the component
    for (XMLElement* pNode = pRoot->FirstChildElement();
          pNode;
          pNode = pNode->NextSiblingElement())
    {
        StrongActorComponentPtr pComponent(VCreateComponent(pNode));
        if (pComponent) {
            pActor->AddComponent(pComponent);
            pComponent->SetOwner(pActor);
        } else {
            return StrongActorPtr();
        }
    }

    if (overrides) {
        ModifyActor(pActor, overrides);
    }

    // This is a bit of a hack to get the initial transform of the
    // transform component set before the other components
    // (like PhysicsComponent) read it
    //TODO
    /*std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(
        pActor->GetComponent<TransformComponent>(TransformComponent::g_Name)
    );
    if (pInitialTransform && pTransformComponent) {
        pTransformComponent->SetPosition(pInitialTransform->GetPosition());
    }*/

    // Now that the actor has been fully created, run the post init phase
    pActor->PostInit();

    return pActor;
}

StrongActorComponentPtr 
ActorFactory::VCreateComponent(XMLElement* pData)
{
    std::string name(pData->Value());
    StrongActorComponentPtr pComponent(
        m_ComponentFactory.Create(
            ActorComponent::GetIdFromName(name.c_str())
        )
    );

    if (pComponent) {
        if (!pComponent->VInit(pData)) {
            throw std::runtime_error("Component failed to initialize: " + name);
        }
    } else {
        throw std::runtime_error("Couldn't find actor component named " + name);
    }

    return pComponent;
}

void ActorFactory::ModifyActor(StrongActorPtr pActor, XMLElement* overrides)
{
    // loop through each child element and load the component
    for (XMLElement* pNode = overrides->FirstChildElement();
        pNode;
        pNode = pNode->NextSiblingElement())
    {
        ComponentId componentId = ActorComponent::GetIdFromName(pNode->Value());
        StrongActorComponentPtr pComponent = MakeStrongPtr(
            pActor->GetComponent<ActorComponent>(componentId)
        );
        if (pComponent) {
            pComponent->VInit(pNode);
            pComponent->VOnChanged();
        } else {
            pComponent = VCreateComponent(pNode);
            if (pComponent) {
                pActor->AddComponent(pComponent);
                pComponent->SetOwner(pActor);
            }
        }
    }
}

#undef DBG_PRINT

