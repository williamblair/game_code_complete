#include <DelayProcess.h>

DelayProcess::DelayProcess( uint64_t timeToDelay )
{
    m_timeToDelay = timeToDelay;
    m_timeDelayedSoFar = 0;
}

void DelayProcess::VOnUpdate( uint64_t deltaMs )
{
    m_timeDelayedSoFar += deltaMs;
    if ( m_timeDelayedSoFar >= m_timeToDelay )
    {
        Succeed();
    }
}
