#include <iostream>
#include <cassert>
#include <Actor.h>
#include <ActorComponent.h>

Actor::Actor( ActorId id )
{
    m_id = id;
    m_type = "Unknown";
}

Actor::~Actor()
{
}

bool Actor::Init( XMLElement* pData )
{
    const char* attrib = pData->Attribute( "type" );
    if ( !attrib ) {
        return false;
    }

    m_type = std::string( attrib );
    return true;
}

void Actor::PostInit()
{
    for ( auto it = m_components.begin(); it != m_components.end(); ++it )
    {
        it->second->VPostInit();
    }
}

void Actor::Destroy()
{
    m_components.clear();
}

void Actor::Update( int deltaMs )
{
    for ( auto it = m_components.begin(); it != m_components.end(); ++it )
    {
        it->second->VUpdate( deltaMs );
    }
}

void Actor::PrintComponents()
{
    std::cout << "Actor components:" << std::endl;
    for ( auto it = m_components.begin(); it != m_components.end(); ++it )
    {
        std::cout << "  " << it->second->VGetPrintInfo() << std::endl;
    }
}

void Actor::AddComponent( StrongActorComponentPtr pComponent )
{
    std::pair<std::map<ComponentId, StrongActorComponentPtr>::iterator,
              bool> 
        success = m_components.insert(
            std::make_pair(pComponent->VGetId(), pComponent));
    assert(success.second);
}

