#include <cstdio>

#include <GCC4/GCCRandom.h>
#include <GCC4/ResCache.h>
#include <GCC4/GameCodeApp.h>
#include <GCC4/SocketManager.h>

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
BaseSocketManager sockMgr; // internally sets g_pSocketManager

int main(void)
{
    GCCRandom randGen;
    randGen.Randomize(); // init seed

    // gen some random values
    for (int i = 0; i < 10; ++i) {
        printf("Rand val %d: %u\n", i, randGen.Random(100));
    }

    return 0;
}

