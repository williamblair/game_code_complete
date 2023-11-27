#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <map>

#include <GCC4/ProcessManager.h>
#include <GCC4/ConcurrentQueue.h>
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

void testProtectedProcess()
{
    ProcessManager processManager;

    printf("Begin test protected process\n");
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
    printf("End testProtectedProcess\n");
}

void testConcurrentQueue()
{
    ConcurrentQueue<int> queue;
    printf("Begin testConcurrentQueue\n");

    std::thread pushThread = std::thread([&] () {
        for (int i=0; i<10; ++i) {
            queue.push(i);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    int queueReadCount = 0;
    while (queueReadCount < 10)
    {
        int val;
        queue.wait_and_pop(val);
        printf("Read queue val: %d\n", val);
        ++queueReadCount;
    }
    pushThread.join();
    printf("End testConcurrentQueue\n");
}

int main()
{
    testProtectedProcess();
    testConcurrentQueue();

    return 0;
}

