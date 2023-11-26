#ifndef AMMO_PICKUP_H_INCLUDED
#define AMMO_PICKUP_H_INCLUDED

#include "PickupInterface.h"

class AmmoPickup : public PickupInterface
{
public:

    static const char * const sName;// = "AmmoPickup";

    ComponentId VGetComponentId() const {
        void* rawId = HashedString::hash_name(sName);
        return reinterpret_cast<ComponentId>(rawId);
    }

    virtual tinyxml2::XMLElement* VGenerateXml();

    virtual bool VInit(tinyxml2::XMLElement* pData);
    virtual void VApply(WeakActorPtr pActor);
    
    virtual const char* VGetName() const { return sName; }

    virtual std::string VGetPrintInfo();

private:

    int m_numAmmo;

};

#endif // AMMO_PICKUP_H_INCLUDED

