#include "IGamePhysics.h"
#include "BulletPhysics.h"

IGamePhysics* CreateGamePhysics()
{
    IGamePhysics* pGamePhysics = new BulletPhysics;
    if (!pGamePhysics || !pGamePhysics->VInitialize()) {
        if (pGamePhysics) {
            delete pGamePhysics;
        }
        return nullptr;
    }
    return pGamePhysics;
}

