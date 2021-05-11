#include <iostream>
#include <memory>
#include <Actor.h>
#include <ActorFactory.h>

int main(void)
{
    ActorFactory actorFactory;

    StrongActorPtr actor = actorFactory.CreateActor( "TreasureChest.xml" );
    if ( !actor ) {
        std::cerr << "Failed to create actor" << std::endl;
        return 1;
    }

    actor->PrintComponents();

    actor->Destroy(); // important to free memory

    return 0;
}

