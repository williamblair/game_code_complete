#include <GCC4/Logger.h>
#include <GCC4/GCCTime.h>
#include "TeapotWarsApp.h"
#include "TeapotWarsLogic.h"

TeapotWarsApp g_TeapotWarsApp;

void MainLoop()
{
    uint32_t timeCounter = GCC4Time::timeGetTime();
    while (g_pApp->IsRunning())
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            GameCodeApp::MsgProc(e);
            if (e.type == SDL_QUIT) {
                return;
            }
        }
        uint32_t curTime = GCC4Time::timeGetTime();
        uint32_t timeDiff = curTime - timeCounter;
        GameCodeApp::OnUpdateGame(float(timeDiff), float(timeCounter));
        timeCounter += timeDiff;

        GameCodeApp::OnFrameRender(float(timeDiff), float(timeCounter));
    }
}

int GameCode4(int argc, char* argv[])
{
    Logger::Init("logging.xml");
    g_pApp->m_Options.Init("PlayerOptions.xml", argc, argv);
    if (!g_pApp->Init("Teapot Wars", 640, 480)) {
        return 1;
    }
    
    MainLoop();
    Logger::Destroy();
    return g_pApp->GetExitCode();
}

#ifdef _WIN32
#undef main
#endif

int main(int argc, char* argv[])
{
    return GameCode4(argc, argv);
}

