#ifndef GCC4_DECOMPRESSION_PROCESS_H_INCLUDED
#define GCC4_DECOMPRESSION_PROCESS_H_INCLUDED

#include "RealtimeProcess.h"

class DecompressionProcess : public RealtimeProcess
{
public:

    DecompressionProcess();
    virtual ~DecompressionProcess();

    virtual void VThreadProc();

    ThreadSafeEventQueue m_realtimeEventQueue;

    void DecompressRequestDelegate(IEventDataPtr pEventData);
    EventListenerDelegate m_DecompressRequestDelegate;
    bool m_bRunThread;
};

#endif // GCC4_DECOMPRESSION_PROCESS_H_INCLUDED

