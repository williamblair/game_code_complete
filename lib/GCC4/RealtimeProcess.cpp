#include "RealtimeProcess.h"

void RealtimeProcess::VOnInit()
{
    Process::VOnInit();
    m_thread = std::thread(&RealtimeProcess::ThreadProc, this);
    //TODO
    //SetThreadPriority(m_thread,m_threadPrio);
}

void RealtimeProcess::ThreadProc(void* pParam)
{
    RealtimeProcess* pProc = static_cast<RealtimeProcess*>(pParam);
    pProc->VThreadProc();
}



ThreadSafeEventQueue EventReaderProcess::m_realtimeEventQueue;

EventReaderProcess::EventReaderProcess() :
    m_eventsRead(0)
{}

void EventReaderProcess::UpdateTickDelegate(IEventDataPtr pEventData)
{
    IEventDataPtr pEventClone = pEventData->VCopy();
    m_realtimeEventQueue.push(pEventClone);
}

void EventReaderProcess::VThreadProc()
{
    // wait for all threads to end
    while (m_eventsRead < 100000) // TODO - clean shutdown
    {
        IEventDataPtr e;
        if (m_realtimeEventQueue.try_pop(e)) {
            ++m_eventsRead;
        }
    }
    Succeed();
}

