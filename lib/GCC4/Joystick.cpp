#include <vector>
#include "Joystick.h"

#include <SDL2/SDL.h>

class ControllerWrapper
{
public:
    ControllerWrapper(SDL_GameController* pCtrl) :
        m_pCtrl(pCtrl)
    {}
    ~ControllerWrapper()
    {
        printf("ControllerWrapper dtor\n");
        if (m_pCtrl) {
            SDL_GameControllerClose(m_pCtrl);
        }
    }
private:
    SDL_GameController* m_pCtrl;
};
static std::vector<ControllerWrapper> g_Controllers;
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
    g_Controllers.reserve(numJoysticks);
    for (int i=0; i<numJoysticks; ++i) {
        SDL_GameController* pCtrl = SDL_GameControllerOpen(i);
        if (!pCtrl) {
            printf("Failed to open game controller %d\n", i);
            return false;
        }
        g_Controllers.push_back(ControllerWrapper(pCtrl));
    }
    printf("g_Controllers size: %d\n", (int)g_Controllers.size());
    return true;
}

