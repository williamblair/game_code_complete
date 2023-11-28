#ifndef GCC4_PROCESS_H_INCLUDED
#define GCC4_PROCESS_H_INCLUDED

#include <cstdint>
#include <cassert>
#include <iostream>
#include "ProcessTypes.h"


#define DBG_ASSERT(cond) assert(cond)
//#define DBG_ASSERT(cond)

class Process
{

friend class ProcessManager;

public:

    enum class State
    {
        // Processes neither dead or alive
        UNINITIALIZED = 0, // created but not running
        REMOVED = 1,       // removed from process list but not destroyed; can
                           //   happen when a process that is already running
                           //   is parented to another process

        // Living processes
        RUNNING = 2,       // Initialized and running
        PAUSED = 3,        // Initialized but paused

        // Dead processes
        SUCCEEDED = 4,     // completed successfully
        FAILED = 5,        // failed to complete
        ABORTED = 6        // may not have started
    };

    Process();
    virtual ~Process();

    // Functions for ending the process
    inline void Succeed();
    inline void Fail();

    // Functions for pausing the process
    inline void Pause();
    inline void UnPause();

    // accessors
    State GetState() const { return m_state; }
    bool IsAlive() const { 
        return (m_state == State::RUNNING || m_state == State::PAUSED);
    }
    bool IsDead() const {
        return (m_state == State::SUCCEEDED || 
            m_state == State::FAILED || 
            m_state == State::ABORTED);
    }
    bool IsPaused() const {
        return (m_state == State::PAUSED);
    }
    bool IsRemoved() const {
        return (m_state == State::REMOVED);
    }

    // child functions
    inline void AttachChild(StrongProcessPtr pChild);
    StrongProcessPtr RemoveChild();
    StrongProcessPtr PeekChild() { return m_pChild; }

protected:

    // interface functions; to be overridden by subclass
    virtual void VOnInit() { m_state = State::RUNNING; }
    virtual void VOnUpdate(uint64_t deltaMs) = 0;
    virtual void VOnSuccess() {}
    virtual void VOnFail() {}
    virtual void VOnAbort() {}

private:

    State m_state;
    // Once the current process exists successfully (via Success()),
    // if this child process exists, it will be promoted and added to the
    // ProcessManager's list and be ran next
    //
    // If the child process exists, but the current process failed via
    // Fail() or was aborted, it will NOT be added and ran
    StrongProcessPtr m_pChild; // child process, if any

    void SetState(State newState) { m_state = newState; }
};

inline void Process::Succeed()
{
    DBG_ASSERT(m_state == State::RUNNING || m_state == State::PAUSED);
    m_state = State::SUCCEEDED;
}

inline void Process::Fail()
{
    DBG_ASSERT(m_state == State::RUNNING || m_state == State::PAUSED);
    m_state = State::FAILED;
}

inline void Process::Pause()
{
    if (m_state == State::RUNNING) {
        m_state = State::PAUSED;
    } else {
        std::cout << __func__ 
            << ": warning: pause called on process not running"
            << std::endl;
    }
}

inline void Process::UnPause()
{
    if (m_state == State::PAUSED) {
        m_state = State::RUNNING;
    } else {
        std::cout << __func__ 
            << ": warning: unpause called on process not paused"
            << std::endl;
    }
}

inline void Process::AttachChild(StrongProcessPtr pChild)
{
    if (m_pChild) {
        m_pChild->AttachChild(pChild);
    } else {
        m_pChild = pChild;
    }
}

#undef DBG_ASSERT

#endif // PROCESS_H_INCLUDED

