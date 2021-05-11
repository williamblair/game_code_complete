#ifndef ACTOR_FACTORY_H_INCLUDED
#define ACTOR_FACTORY_H_INCLUDED

#include <map>
#include <memory>

#include <Actor.h>
#include <ActorComponent.h>

typedef ActorComponent *(*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

typedef std::shared_ptr<Actor> StrongActorPtr;
typedef std::shared_ptr<Component> StrongActorComponentPtr;

class ActorFactory
{
public:

    ActorFactory();

    /**
     * parses the XML file indicated by actorResourceXML
     * and returns an actor pointer to it
     */
    StrongActorPtr CreateActor( const char* actorResourceXML );

private:
    ActorId m_lastActorId;
    ActorComponentCreatorMap m_actorComponentCreators;

    virtual StrongActorComponentPtr CreateComponent( TiXmlElement* pData );
    ActorId GetNextActorId() { ++m_lastActorId; return m_lastActorId; }
};

#endif // ACTOR_FACTORY_H_INCLUDED

