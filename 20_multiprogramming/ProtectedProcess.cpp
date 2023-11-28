#include "ProtectedProcess.h"

uint32_t ProtectedProcess::g_ProtectedTotal = 0;
CriticalSection ProtectedProcess::g_CriticalSection;

void ProtectedProcess::VThreadProc()
{
    uint32_t count = 0;

    while (count < m_MaxLoops)
    {
       ++count;

        {
            // Request ownership of critical section
            ScopedCriticalSection locker(g_CriticalSection);
            ++g_ProtectedTotal;
        } 
    }
    Succeed();
}


