#include <cstdio>
#include <memory>

#include <GCC4/Actor.h>
#include <GCC4/ActorFactory.h>

#include "AmmoPickup.h"
#include "HealthPickup.h"

int main()
{
    ActorFactory actorFactory;
    actorFactory.m_ComponentFactory.Register<AmmoPickup>(
        ActorComponent::GetIdFromName(AmmoPickup::sName)
    );
    actorFactory.m_ComponentFactory.Register<HealthPickup>(
        ActorComponent::GetIdFromName(HealthPickup::sName)
    );

    StrongActorPtr actor = actorFactory.CreateActor(
        "TreasureChest.xml",
        nullptr, // tinyxml2::XMLElement* overrides,
        nullptr, // const Mat4x4* initialTransform,
        INVALID_ACTOR_ID // const ActorId serversActorId
    );
    if (!actor) {
        printf("Failed to create actor\n");
        return 1;
    }

    actor->PrintComponents();
    actor->Destroy(); // important to free memory

    return 0;
}

