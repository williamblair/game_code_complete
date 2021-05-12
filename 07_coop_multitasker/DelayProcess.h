#ifndef DELAY_PROCESS_H_INCLUDED
#define DELAY_PROCESS_H_INCLUDED

#include <cstdint>
#include <Process.h>

class DelayProcess : public Process
{
public:

    explicit DelayProcess( uint64_t timeToDelay );

protected:

    virtual void VOnUpdate( uint64_t deltaMs );

private:

    uint64_t m_timeToDelay;
    uint64_t m_timeDelayedSoFar;
};

#endif // DELAY_PROCESS_H_INCLUDED

