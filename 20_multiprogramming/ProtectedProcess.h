#ifndef PROTECTED_PROCESS_H_INCLUDED
#define PROTECTED_PROCESS_H_INCLUDED

#include <cstdint>

#include <GCC4/CriticalSection.h>
#include <GCC4/RealtimeProcess.h>

class ProtectedProcess : public RealtimeProcess
{
public:
    static uint32_t g_ProtectedTotal;
    static CriticalSection g_CriticalSection;
    uint32_t m_MaxLoops;
    
    ProtectedProcess(uint32_t maxLoops) :
        RealtimeProcess()
    {
        m_MaxLoops = maxLoops;
    }

    virtual void VThreadProc();
};

#endif // PROTECTED_PROCESS_H_INCLUDED

