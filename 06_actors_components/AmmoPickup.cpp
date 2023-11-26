#include <iostream>
#include <vector>
#include <string>
#include "AmmoPickup.h"
#include <GCC4/MakeStrongPtr.h>
#include <GCC4/Actor.h>

using namespace tinyxml2;

const char * const AmmoPickup::sName = "AmmoPickup";

XMLElement* AmmoPickup::VGenerateXml()
{
    // TODO
    return nullptr;
}

bool AmmoPickup::VInit(XMLElement* pData)
{
    m_numAmmo = -1;
    const char* amountAttrib = pData->Attribute("amount");
    if (!amountAttrib) {
        std::cout << __func__ << ": ammo pickup: no amount attribute found"
                  << std::endl;
        return false;
    }
    try {
        m_numAmmo = std::stoi(amountAttrib);
    } catch (std::invalid_argument& error) {
        std::cout << __func__ << ": failed to convert ammo pickup amount: "
                  << amountAttrib << std::endl;
        return false;
    }
    std::cout << "Init ammo pickup with amount: " << m_numAmmo << std::endl;
    return true;
}

void AmmoPickup::VApply(WeakActorPtr pActor)
{
    StrongActorPtr pStrongActor = MakeStrongPtr(pActor);
    if (pStrongActor) {
        std::cout << "Apply ammo pickup to actor with id: "
                  << pStrongActor->GetId() << std::endl;
    }
}

std::string AmmoPickup::VGetPrintInfo()
{
    return "Ammo Pickup: amount: " + std::to_string(m_numAmmo);
}

