#ifndef AMMO_PICKUP_H_INCLUDED
#define AMMO_PICKUP_H_INCLUDED

#include <PickupInterface.h>
#include <ComponentIds.h>

class AmmoPickup : public PickupInterface
{
public:

    static ActorComponent* Create();

    static const ComponentId COMPONENT_ID = 
        ComponentId(COMPONENT_IDS::AMMO_PICKUP); // unique ID for this component type
    ComponentId VGetComponentId() const { return COMPONENT_ID; }

    virtual bool VInit( XMLElement* pData );
    virtual void VApply( WeakActorPtr pActor );

    virtual std::string VGetPrintInfo();

private:

    int m_numAmmo;

};

#endif // AMMO_PICKUP_H_INCLUDED

