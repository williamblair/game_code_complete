#include <iostream>

#include <GCC4/ResCache.h>
#include <GCC4/GameCodeApp.h>

#include "App.h"
#include "ScriptExports.h"
#include "LuaStateManager.h"

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

int main(void)
{
    App theApp;
    g_pTestApp = &theApp;

    if (!g_pTestApp->Init())
    {
        std::cout << __FILE__ << ":" << __LINE__
                  << ": failed to init app" << std::endl;
        return 1;
    }

    g_pTestApp->Run();
    
    return 0;
}

