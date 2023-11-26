#include <chrono>
#include <thread>

#include <GCC4/Process.h>
#include <GCC4/ProcessManager.h>

#include "DelayProcess.h"
#include "KaboomProcess.h"

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
    StrongProcessPtr pDelayProcess(new DelayProcess(5000)); // 5 second delay
    StrongProcessPtr pKaboomProcess(new KaboomProcess()); // gets called after delay
    
    processManager.AttachProcess(pDelayProcess);
    pDelayProcess->AttachChild(pKaboomProcess);

    auto timeStart = std::chrono::steady_clock::now();
    auto lastTime = std::chrono::steady_clock::now();
    auto timeStop = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>
            (timeStop - timeStart).count() < 10)
    {
        timeStop = std::chrono::steady_clock::now();
        uint64_t deltaMs = std::chrono::duration_cast<std::chrono::milliseconds>
            (timeStop - lastTime).count();

        processManager.UpdateProcesses(deltaMs);

        // go easy on the CPU a little
        lastTime = timeStop;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(uint64_t(1000.0f/30.0f)));
    }

    return 0;
}

