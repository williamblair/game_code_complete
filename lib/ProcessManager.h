#ifndef PROCESS_MANAGER_H_INCLUDED
#define PROCESS_MANAGER_H_INCLUDED

#include <cstdint>
#include <list>

#include <ProcessTypes.h>

class ProcessManager
{

    typedef std::list<StrongProcessPtr> ProcessList;

public:

    ProcessManager();
    ~ProcessManager();

    uint32_t UpdateProcesses( uint64_t deltaMs );
    WeakProcessPtr AttachProcess( StrongProcessPtr pProcess );
    void AbortAllProcesses( bool immediate );

    uint32_t GetProcessCount() const { return m_processList.size(); }

private:

    ProcessList m_processList;

    void ClearAllProcesses(); // should only be called by destructor
};

#endif // PROCESS_MANAGER_H_INCLUDED

