//#include <SDL2/SDL.h>
#include <chrono>
#include "GCCTime.h"

namespace GCC4Time
{

static auto timeStart = std::chrono::steady_clock::now();
uint32_t timeGetTime() {
    auto timeNow = std::chrono::steady_clock::now();
    uint32_t runTime = (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - timeStart).count();
    return runTime;
}

}
