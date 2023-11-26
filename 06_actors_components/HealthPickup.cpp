#include <iostream>

#include "HealthPickup.h"
#include <GCC4/MakeStrongPtr.h>
#include <GCC4/Actor.h>

using namespace tinyxml2;

const char * const HealthPickup::sName = "HealthPickup";

XMLElement* HealthPickup::VGenerateXml()
{
    //TODO
    return nullptr;
}

bool HealthPickup::VInit(XMLElement* pData)
{
    m_numHealth = -1;
    const char* amountAttrib = pData->Attribute("amount");
    if (!amountAttrib) {
        std::cout << __func__ << ": health pickup: no amount attribute found"
                  << std::endl;
        return false;
    }
    try {
        m_numHealth = std::stoi(amountAttrib);
    } catch (std::invalid_argument& error) {
        std::cout << __func__ << ": failed to convert health pickup amount: "
                  << amountAttrib << std::endl;
        return false;
    }
    std::cout << "Init health pickup with amount: " << m_numHealth << std::endl;
    return true;
}

void HealthPickup::VApply(WeakActorPtr pActor)
{
    StrongActorPtr pStrongActor = MakeStrongPtr(pActor);
    if (pStrongActor)
    {
        std::cout << "Apply health pickup to actor with id: "
                  << pStrongActor->GetId() << std::endl;
    }
}

std::string HealthPickup::VGetPrintInfo()
{
    return "health pickup: amount: " + std::to_string(m_numHealth);
}

