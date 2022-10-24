#include "GCCTime.h"
#include <SDL2/SDL.h>

uint32_t timeGetTime() { return SDL_GetTicks(); }
