#include <ProcessManager.h>
#include <Process.h>

ProcessManager::ProcessManager()
{
}

ProcessManager::~ProcessManager()
{
    m_processList.clear();
}

uint32_t ProcessManager::UpdateProcesses( uint64_t deltaMs )
{
    uint16_t successCount = 0;
    uint16_t failCount = 0;
    
    ProcessList::iterator it = m_processList.begin();
    while ( it != m_processList.end() )
    {
        StrongProcessPtr pCurProcess = *it;
        ProcessList::iterator thisIt = it;
        ++it;

        if ( pCurProcess->GetState() == Process::State::UNINITIALIZED )
        {
            pCurProcess->VOnInit();
        }

        if ( pCurProcess->GetState() == Process::State::RUNNING )
        {
            pCurProcess->VOnUpdate( deltaMs );
        }

        if ( pCurProcess->IsDead() )
        {
            switch ( pCurProcess->GetState() )
            {
            case Process::State::SUCCEEDED:
            {
                pCurProcess->VOnSuccess();
                StrongProcessPtr pChild = pCurProcess->RemoveChild();
                if ( pChild )
                {
                    AttachProcess( pChild );
                }
                else
                {
                    ++successCount; // only count if whole chain is completed
                }
                break;
            }
            case Process::State::FAILED:
            {
                pCurProcess->VOnFail();
                ++failCount;
                break;
            }
            case Process::State::ABORTED:
            {
                pCurProcess->VOnAbort();
                ++failCount;
                break;
            }
            default:
            {
                break;
            }
            } // end switch ( pCurProcess->GetState() )

            // remove from process list if dead
            // since process is a smart pointer, it will auto erase itself
            m_processList.erase( thisIt );
        }
    }

    return uint32_t(( successCount << 16 ) | failCount );
}

WeakProcessPtr ProcessManager::AttachProcess( StrongProcessPtr pProcess )
{
    m_processList.push_front( pProcess );
    return WeakProcessPtr( pProcess );
}

void ProcessManager::AbortAllProcesses( bool immediate )
{
    ProcessList::iterator it = m_processList.begin();
    while ( it != m_processList.end() )
    {
        ProcessList::iterator tmpIt = it;
        ++it;

        StrongProcessPtr pProcess = *tmpIt;
        if ( pProcess->IsAlive() )
        {
            pProcess->SetState( Process::State::ABORTED );
            if ( immediate )
            {
                pProcess->VOnAbort();
                m_processList.erase( tmpIt );
            }
        }
    }
}

