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

