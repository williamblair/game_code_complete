#ifndef ACTOR_COMPONENT_H_INCLUDED
#define ACTOR_COMPONENT_H_INCLUDED

#include <string>
#include "xml.h"
#include "ComponentTypes.h"
#include "HashedString.h"

// forward declaration
class ActorFactory;

class ActorComponent
{

friend class ActorFactory;

public:

    virtual ~ActorComponent() { m_pOwner.reset(); }

    /**
     * to be overridden by component implementations
     */
    virtual bool VInit( XMLElement* pData ) = 0;
    virtual void VPostInit() {}
    virtual void VUpdate(int deltaMs) {}
    virtual void VOnChanged() {}

    // For the editor
    //TODO
    //virtual XMLElement* VGenerateXml() = 0;

    // To be overridden by the interface class
    virtual ComponentId VGetId() const { return GetIdFromName(VGetName()); }
    
    virtual const char* VGetName() const = 0;
    static ComponentId GetIdFromName(const char* componentStr) {
        void* rawId = HashedString::hash_name(componentStr);
        return reinterpret_cast<ComponentId>(rawId);
    }

    /**
     * to print debug info
     */
    virtual std::string VGetPrintInfo() = 0;

protected:

    StrongActorPtr m_pOwner;

private:

    void SetOwner(StrongActorPtr owner) { m_pOwner = owner; }

};

#endif // ACTOR_COMPONENT_H_INCLUDED

