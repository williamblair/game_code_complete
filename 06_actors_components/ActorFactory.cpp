#include <ActorFactory.h>

StrongActorPtr ActorFactory::CreateActor(const char* actorResourceXML)
{
    TiXmlElement* pRoot = 
        XmlResourceLoader::LoadAndReturnRootXmlElement(actorResourceXML);
    if ( !pRoot ) {
        std::cerr << __func__ << ": Failed to create actor from resource: " 
                  << actorResourceXML << std::endl;
        return StrongActorPtr();
    }

    StrongActorPtr pActor( new Actor( GetNextActorId() ) );
    if ( !pActor->Init( pRoot ) ) {
        std::cerr << __func__ << ": failed to init actor "
                  << actorResourceXML << std::endl;
        return StrongActorPtr();
    }

    // loop through each child XML element and load the component
    for ( TiXmlElement* pNode = pRoot->FirstChildElement();
          pNode;
          pNode = pNode->NextSiblingElement())
    {
        StrongActorComponentPtr pComponent( CreateComponent( pNode ) );
        if ( pComponent ) {
            pActor->AddComponent( pComponent );
            pComponent->SetOwner( pActor );
        } else {
            return StrongActorPtr();
        }
    }

    pActor->PostInit();

    return pActor;
}

StrongActorComponentPtr 
ActorFactory::CreateComponent( TiXmlElement* pData )
{
    std::string name( pData->Value() );
    StrongActorComponentPtr pComponent;

    // initialize the component from the creator function
    auto componentCreateIter = m_actorComponentCreators.find( name );
    if ( componentCreateIter != m_actorComponentCreators.end() ) {
        ActorComponentCreator creator = componentCreateIter->second;
        pComponent.reset( creator() );
    } else {
        std::cerr << __func__ << ": failed to find ActorComponent named" 
                  << name << std::endl;
        return StrongActorComponentPtr();
    }

    if ( pComponent ) {
        if ( !pComponent->Init( pData ) ) {
            std::cerr << __func__ << ": failed to init component named "
                      << name << std::endl;
            return StrongActorComponentPtr();
        }
    }

    return pComponent;
}

