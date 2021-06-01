#include <iostream>
#include <App.h>
#include <InternalScriptExports.h>
#include <LuaStateManager.h>

int main(void)
{
    App theApp;
    g_pApp = &theApp;

    if ( !g_pApp->Init() )
    {
        std::cout << __FILE__ << ":" << __LINE__
                  << ": failed to init app" << std::endl;
        return 1;
    }

    g_pApp->Run();
    
    return 0;
}

