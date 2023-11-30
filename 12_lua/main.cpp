#include <iostream>

#include <GCC4/GameCodeApp.h>

#include "App.h"

LuaTestApp g_LuaTestApp;

int main(void)
{
    if (!g_LuaTestApp.Init("LuaTestApp",0,0))
    {
        std::cout << __FILE__ << ":" << __LINE__
            << ": failed to init app" << std::endl;
        return 1;
    }

    g_LuaTestApp.Run();
    
    return 0;
}

