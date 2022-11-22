#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED

#include <cstdint>
#include <map>

#include "ComponentTypes.h"
#include "xml.h"
#include "ActorComponent.h"

// forward declaration
class ActorFactory;

class Actor
{

friend class ActorFactory;

public:

    // prevent copy/assignment using non-ActorId ulong value
    explicit Actor( ActorId id );
    ~Actor();

    bool Init( XMLElement* pData );
    void PostInit();
    void Destroy();
    void Update( int deltaMs );

    std::string ToXml();

    ActorId GetId() const { return m_id; }

    template<class ComponentType>
    std::weak_ptr<ComponentType> GetComponent(ComponentId id)
    {
        auto componentIter = m_components.find(id);
        if (componentIter != m_components.end()) {
            StrongActorComponentPtr pBase(componentIter->second);
            // cast to subclass version
            std::shared_ptr<ComponentType> pSub(
                std::static_pointer_cast<ComponentType>(pBase));
            // convert strong ptr to weak ptr
            std::weak_ptr<ComponentType> pWeakSub(pSub);
            return pWeakSub;
        }
        return std::weak_ptr<ComponentType>();
    }
    template<class ComponentType>
    std::weak_ptr<ComponentType> GetComponent(const char* name)
    {
        ComponentId id = ActorComponent::GetIdFromName(name);
        auto componentIter = m_components.find(id);
        if (componentIter != m_components.end())
        {
            StrongActorComponentPtr pBase(componentIter->second);
            std::shared_ptr<ComponentType> pSub(
                std::static_pointer_cast<ComponentType>(pBase)
            );
            std::weak_ptr<ComponentType> pWeakSub(pSub);
            return pWeakSub;
        }
        return std::weak_ptr<ComponentType>();
    }

    void PrintComponents();

private:

    ActorId m_id;
    ActorType m_type;
    std::map<ComponentId, StrongActorComponentPtr> m_components;

    // the XML file from which this actor was initialized
    std::string m_resource; 

    // only the ActorFactory should call this function
    void AddComponent( StrongActorComponentPtr pComponent );
};

#endif // ACTOR_H_INCLUDED

