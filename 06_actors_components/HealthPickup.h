#ifndef HEALTH_PICKUP_H_INCLUDED
#define HEALTH_PICKUP_H_INCLUDED

#include <xml.h>
#include <PickupInterface.h>
#include <ComponentIds.h>

class HealthPickup : public PickupInterface
{
public:

    static ActorComponent* Create();

    static const ComponentId COMPONENT_ID = 
        ComponentId(COMPONENT_IDS::HEALTH_PICKUP); // unique ID for this component type
    ComponentId VGetComponentId() const { return COMPONENT_ID; }

    virtual bool VInit( XMLElement* pData );
    virtual void VApply( WeakActorPtr pActor );

    virtual std::string VGetPrintInfo();

private:

    int m_numHealth;
};

#endif // HEALTH_PICKUP_H_INCLUDED

