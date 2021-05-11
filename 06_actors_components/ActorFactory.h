#ifndef ACTOR_FACTORY_H_INCLUDED
#define ACTOR_FACTORY_H_INCLUDED

#include <map>
#include <memory>

#include <xml.h>

#include <ComponentTypes.h>
#include <Actor.h>
#include <ActorComponent.h>

class ActorFactory
{
public:

    ActorFactory();
    ~ActorFactory();

    /**
     * parses the XML file indicated by actorResourceXML
     * and returns an actor pointer to it
     */
    StrongActorPtr CreateActor( const char* actorResourceXML );

private:
    ActorId m_lastActorId;
    ActorComponentCreatorMap m_actorComponentCreators;

    virtual StrongActorComponentPtr CreateComponent( XMLElement* pData );
    ActorId GetNextActorId() { ++m_lastActorId; return m_lastActorId; }
};

#endif // ACTOR_FACTORY_H_INCLUDED

