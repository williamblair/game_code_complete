#ifndef GCC4_REALTIME_PROCESS_H_INCLUDED
#define GCC4_REALTIME_PROCESS_H_INCLUDED

#include <thread>

#include "Process.h"

class RealtimeProcess : public Process
{
public:
    RealtimeProcess() /*:
        Process(PROC_REALTIME)*/
    {}
    virtual ~RealtimeProcess() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    static void ThreadProc(void* pParam);
    
protected:
    std::thread m_thread;

    virtual void VOnInit();
    virtual void VOnUpdate(unsigned long deltaMs) {}
    virtual void VThreadProc() = 0;
};

#endif // GCC4_REALTIME_PROCESS_H_INCLUDED

