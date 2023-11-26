#include "SoundResourceExtraData.h"
#include "WaveResourceLoader.h"

#include <GCC4/ResCache.h>
#include <GCC4/GameCodeApp.h>

// Required globals - TODO move
ResCache* g_ResCache = nullptr;
class TestGCCApp : public GameCodeApp
{
public:
    virtual const char* VGetGameTitle() { return "TestGCCApp"; }
    virtual const char* VGetGameAppDirectory() { return "./"; }
};
TestGCCApp g_TestGCCApp;
GameCodeApp* g_pApp = &g_TestGCCApp;

int main()
{
    return 0;
}

