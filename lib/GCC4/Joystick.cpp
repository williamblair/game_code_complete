#include "Joystick.h"

#include <SDL2/SDL_gamecontroller.h>

bool CheckForJoystick()
{
    Uint32 initFlags = SDL_WasInit(SDL_INIT_GAMECONTROLLER);
    if (!(initFlags & SDL_INIT_GAMECONTROLLER)) {
        if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
            printf("SDL Init game controller failed: %s\n", SDL_GetError());
            return false;
        }
    }

    const int numJoysticks = SDL_NumJoysticks();
    for (int i=0; i<numJoysticks; ++i) {
        //TODO
        SDL_GameController* pCtrl = SDL_GameControllerOpen(i);
        if (!pCtrl) {
            printf("Failed to open game controller %d\n", i);
            return false;
        }
    }
}

