#ifndef PICKUP_INTERFACE_H_INCLUDED
#define PICKUP_INTERFACE_H_INCLUDED

#include <ActorComponent.h>

/**
 * Actor component type that can be picked up by the player
 */
class PickupInterface : public ActorComponent
{
public:

    virtual ComponentId VGetComponentId() const = 0;
    virtual void VApply( WeakActorPtr pActor ) = 0;

    virtual std::string VGetPrintInfo() = 0;
};

#endif // PICKUP_INTERFACE_H_INCLUDED

