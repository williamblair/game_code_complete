#ifndef ACTOR_COMPONENT_H_INCLUDED
#define ACTOR_COMPONENT_H_INCLUDED

#include <string>
#include "xml.h"
#include "ComponentTypes.h"

// forward declaration
class ActorFactory;

class ActorComponent
{

friend class ActorFactory;

public:

    virtual ~ActorComponent() {}

    /**
     * to be overridden by component implementations
     */
    virtual bool VInit( XMLElement* pData ) = 0;
    virtual void VPostInit() {}
    virtual void VUpdate( int deltaMs ) {}
    virtual ComponentId VGetComponentId() const = 0;

    /**
     * to print debug info
     */
    virtual std::string VGetPrintInfo() = 0;

protected:

    StrongActorPtr m_pOwner;

private:

    void SetOwner( StrongActorPtr owner ) { m_pOwner = owner; }

};

#endif // ACTOR_COMPONENT_H_INCLUDED

