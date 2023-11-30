#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <map>

#include <GCC4/ProcessManager.h>
#include <GCC4/ConcurrentQueue.h>
#include <GCC4/DecompressionProcess.h>
#include "ProtectedProcess.h"

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

void fileDecompressDelegate(IEventDataPtr pEvent)
{
    std::shared_ptr<EvtDataDecompressProgress> pDecProg =
        std::static_pointer_cast<EvtDataDecompressProgress>(pEvent);
    printf("Decompress progress event:\n");
    printf("    progressPercent: %d\n", pDecProg->m_progressPercent);
    printf("    zipFileName: %s\n", pDecProg->m_zipFileName.c_str());
    printf("    fileName: %s\n", pDecProg->m_fileName.c_str());
    printf("    dataBuffer: 0x%x\n", (unsigned long int)pDecProg->m_dataBuffer);
    printf("    data: %s\n", pDecProg->m_dataBuffer);
    delete[] pDecProg->m_dataBuffer;
}

void testDecompress()
{
    ProcessManager processManager;
    std::unique_ptr<EventManager> pEventMgr(
        new EventManager("EventManager", true)
    );
    StrongProcessPtr pDecProc(new DecompressionProcess());
    bool decompressFired = false;
    printf("Starting testDecompress\n");

    EventListenerDelegate decompressProgressDelegate(
        new std::function<void(IEventDataPtr)>(fileDecompressDelegate)
    );
    IEventManager::GetInstance()->VAddListener(
        decompressProgressDelegate,
        EvtDataDecompressProgress::sk_EventType
    );

    processManager.AttachProcess(pDecProc);

    auto timeStart = std::chrono::steady_clock::now();
    auto lastTime = std::chrono::steady_clock::now();
    auto timeStop = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>
            (timeStop - timeStart).count() < 10)
    {
        timeStop = std::chrono::steady_clock::now();
        uint64_t deltaMs = std::chrono::duration_cast<std::chrono::milliseconds>
            (timeStop - lastTime).count();

        // Update managers
        processManager.UpdateProcesses(deltaMs);
        pEventMgr->VTickUpdate();

        uint64_t msPassed = std::chrono::duration_cast<std::chrono::milliseconds>
            (timeStop - timeStart).count();

        // Request buffer decompression
        if (msPassed > 5000 && !decompressFired) {
            decompressFired = true;
            printf("Firing decompress request\n");
            IEventDataPtr evt(new EvtDataDecompressRequest(
                    "../08_zip/testfile.zip",
                    "testfile.txt"
                )
            );
            pEventMgr->VQueueEvent(evt);
        }

        // go easy on the CPU a little
        lastTime = timeStop;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(uint64_t(1000.0f/30.0f)));
    }

    pDecProc.reset();
    IEventManager::GetInstance()->VRemoveListener(
        decompressProgressDelegate,
        EvtDataDecompressProgress::sk_EventType
    );
    printf("End testDecompress\n");
}

int main()
{
    testProtectedProcess();
    testConcurrentQueue();
    testDecompress();

    return 0;
}

