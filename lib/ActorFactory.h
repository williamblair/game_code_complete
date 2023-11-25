#ifndef GCC4_ACTOR_FACTORY_H_INCLUDED
#define GCC4_ACTOR_FACTORY_H_INCLUDED

#include <map>
#include <memory>

#include "xml.h"

#include "ComponentTypes.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "GenericFactory.h"
#include "GCCMath.h"

class ActorFactory
{
public:

    ActorFactory();
    virtual ~ActorFactory();

    /**
     * parses the XML file indicated by actorResourceXML
     * and returns an actor pointer to it
     */
    StrongActorPtr CreateActor(
        const char* actorResourceXML,
        tinyxml2::XMLElement* overrides,
        const Mat4x4* initialTransform,
        const ActorId serversActorId
    );

    void ModifyActor(StrongActorPtr pActor, tinyxml2::XMLElement* overrides);

    // this function can be overwritten by a subclass so you can create
    // game specific c++ components. If you do this, make sure you call the
    // base class version first. If it returns null, you know it's not an
    // engine component.
    virtual StrongActorComponentPtr VCreateComponent(tinyxml2::XMLElement* pData);

protected:
    GenericObjectFactory<ActorComponent,ComponentId> m_ComponentFactory;

private:
    ActorId m_LastActorId;
    ActorComponentCreatorMap m_ActorComponentCreators;

    
    ActorId GetNextActorId() { ++m_LastActorId; return m_LastActorId; }
};

#endif // GCC4_ACTOR_FACTORY_H_INCLUDED

