#include "EventManager.h"
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>

#include "GCCTime.h"

static IEventManager* pEventManagerInstance = nullptr;

IEventManager::IEventManager(const char* pName, bool setAsGlobal)
{
    if (setAsGlobal) {
        pEventManagerInstance = this;
    }
}

IEventManager::~IEventManager()
{
}

IEventManager* IEventManager::GetInstance()
{
    return pEventManagerInstance;
}

EventManager::EventManager(const char* pName, bool setAsGlobal) :
    IEventManager(pName, setAsGlobal)
{
    m_activeQueue = 0;
}

bool EventManager::VAddListener(
    const EventListenerDelegate& eventDelegate,
    const EventType& type)
{
    EventListenerList& eventListenerList = m_eventListeners[type];
    for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
    {
        if (eventDelegate == (*it)) {
            std::cout << __FILE__ << ":" << __LINE__ 
                      << ": attempting to double register a delegate" << std::endl;
            return false;
        }
    }

    eventListenerList.push_back(eventDelegate);
    return true;
}

bool EventManager::VRemoveListener(
    const EventListenerDelegate& eventDelegate,
    const EventType& type)
{
    bool success = false;

    auto findIt = m_eventListeners.find(type);
    if (findIt != m_eventListeners.end())
    {
        EventListenerList& listeners = findIt->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it)
        {
            if (eventDelegate == (*it))
            {
                listeners.erase(it);
                success = true;

                // should be impossible for any more entries sense we can't
                // register the same delegate twice
                break;
            }
        }
    }

    return success;
}

bool EventManager::VTriggerEvent(const IEventDataPtr& pEvent) const
{
    bool processed = false;

    auto findIt = m_eventListeners.find(pEvent->VGetEventType());
    if (findIt != m_eventListeners.end())
    {
        const EventListenerList& eventListenerList = findIt->second;
        for (EventListenerList::const_iterator it = eventListenerList.begin();
             it != eventListenerList.end();
             ++it)
        {
            EventListenerDelegate listener = *it;
            (*listener)(pEvent);
            processed = true;
        }
    }

    return processed;
}

bool EventManager::VQueueEvent(const IEventDataPtr& pEvent)
{
    assert(m_activeQueue >= 0);
    assert(size_t(m_activeQueue) < EVTMGR_NUM_QUEUES);

    auto findIt = m_eventListeners.find(pEvent->VGetEventType());
    if (findIt != m_eventListeners.end())
    {
        m_queues[m_activeQueue].push_back(pEvent);
        std::cout << "Added event" << std::endl;
        return true;
    }
    else
    {
        std::cout << "No listeners for event w type: 0x"
                  << std::hex << (unsigned long long)pEvent->VGetEventType()
                  << std::dec << std::endl;
    }
    return false;
}

bool EventManager::VThreadSafeQueueEvent(const IEventDataPtr& pEvent)
{
    m_realtimeEventQueue.push(pEvent);
    return true;
}

bool EventManager::VAbortEvent(const EventType& type, bool allOfType)
{
    assert(m_activeQueue >= 0);
    assert(size_t(m_activeQueue) < EVTMGR_NUM_QUEUES);

    bool success = false;
    auto findIt = m_eventListeners.find(type);
    if (findIt != m_eventListeners.end())
    {
        EventQueue& eventQueue = m_queues[m_activeQueue];
        auto it = eventQueue.begin();
        while (it != eventQueue.end())
        {
            // removing item invalidates iterator, so we need to save the next entry
            auto thisIt = it;
            ++it;
            if ((*thisIt)->VGetEventType() == type)
            {
                eventQueue.erase(thisIt);
                success = true;
                // stop if we only wanted to delete one event
                if (!allOfType)
                {
                    break;
                }
            }
        }
    }

    return success;
}

bool EventManager::VTickUpdate(unsigned long maxMillis)
{
    auto startTime = std::chrono::steady_clock::now();

    // Handle events from other threads
    IEventDataPtr pRealtimeEvent;
    unsigned long curMs = GCC4Time::timeGetTime();
    unsigned long maxMs = (maxMillis == IEventManager::kINFINITE) ?
        IEventManager::kINFINITE : (curMs + maxMillis);
    while (m_realtimeEventQueue.try_pop(pRealtimeEvent))
    {
        VQueueEvent(pRealtimeEvent);
        curMs = GCC4Time::timeGetTime();
        if (maxMillis != IEventManager::kINFINITE) {
            if (curMs >= maxMs) {
                printf("A realtime process is spamming the event manager!\n");
            }
        }
    }

    // swap active queues and clear the new queue after swap
    int queueToProcess = m_activeQueue;
    m_activeQueue = (m_activeQueue + 1) % EVTMGR_NUM_QUEUES;
    m_queues[m_activeQueue].clear();

    // process the queue
    while (!m_queues[queueToProcess].empty())
    {
        // Remove the next event from the queue
        IEventDataPtr pEvent = m_queues[queueToProcess].front();
        m_queues[queueToProcess].pop_front();
        const EventType& eventType = pEvent->VGetEventType();

        // find all listeners for the current event
        auto findIt = m_eventListeners.find(eventType);
        if (findIt != m_eventListeners.end())
        {
            const EventListenerList& eventListeners = findIt->second;

            // call each listener
            for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
            {
                EventListenerDelegate listener = *it;
                (*listener)(pEvent);
            }
        }
        else
        {
            std::cout << __FILE__ << ":" << __LINE__ << ": "
                << "failed to find event listeners for type: 0x"
                << std::hex << (unsigned long long)eventType << std::endl
                << std::dec;
        }

        // check if we ran out of time
        if (maxMillis != kINFINITE)
        {
            auto timeNow = std::chrono::steady_clock::now();
            if ((unsigned long)
                std::chrono::duration_cast<std::chrono::milliseconds>( 
                    timeNow - startTime).count() >= maxMillis)
            {
                break;
            }
        }
    }

    // if there are still events left unprocessed, add them to the next queue
    bool queueFlushed = m_queues[queueToProcess].empty();
    if (!queueFlushed)
    {
        while (!m_queues[queueToProcess].empty())
        {
            // add them to the front of the next queue so they will be processed first
            IEventDataPtr pEvent = m_queues[queueToProcess].back();
            m_queues[queueToProcess].pop_back();
            m_queues[m_activeQueue].push_front(pEvent);
        }
    }


    return queueFlushed;
}





