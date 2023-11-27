#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <map>

#include <GCC4/ProcessManager.h>
#include "ProtectedProcess.h"

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
    ProcessManager processManager;

    for (int i=0; i<20; ++i) {
        StrongProcessPtr proc = std::make_shared<ProtectedProcess>(100000);
        processManager.AttachProcess(proc);
    }
    
    printf("Running...\n");
    for (int i=0; i<300; ++i) {
        processManager.UpdateProcesses(1000/30);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/30));
    }

    printf("Stopping, g_ProtectedTotal: %u\n", ProtectedProcess::g_ProtectedTotal);
    assert(ProtectedProcess::g_ProtectedTotal == 2000000);

    return 0;
}

