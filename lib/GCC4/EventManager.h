#ifndef GCC4_EVENT_MANAGER_H_INCLUDED
#define GCC4_EVENT_MANAGER_H_INCLUDED

#include <list>
#include <map>
#include <string>

#include "ConcurrentQueue.h"
#include "EventData.h"


class IEventManager
{
public:

    enum eConstants { kINFINITE = 0xFFFFFFFF }; // amount of time update func has to run

    explicit IEventManager(const char* pName, bool setAsGlobal);
    virtual ~IEventManager();

    // Registers a delegate function that will get called when the event type is triggered
    // returns true if successful, false otherwise
    virtual bool VAddListener(
        const EventListenerDelegate& eventDelegate,
        const EventType& type
    ) = 0;

    // Removes delegate/event pairing from the internal tables.
    // returns false if pairing not found
    virtual bool VRemoveListener(
        const EventListenerDelegate& eventDelegate,
        const EventType& type
    ) = 0;

    // Fire off the event right now; bypasses queue
    virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const = 0;

    // Add event to event queue; will be triggered next update or future update if
    // update func runs past update time limit
    virtual bool VQueueEvent(const IEventDataPtr& pEvent) = 0;

    // Add event to event queue from another thread
    virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent) = 0;

    // finds next-available instance of the named event type and remove it, or all
    // events of the given type if allOfType is true
    // returns true if event found and remove, false otherwise
    virtual bool VAbortEvent(const EventType& type, bool allOfType = false) = 0;

    // Triggers any queued events up to the given input time to execute
    // (default = unlimited time). Any remaining events after the time limit
    // expires remain in the queue.
    virtual bool VTickUpdate(unsigned long maxMillis = kINFINITE) = 0;

    // Get singleton event manager instance.
    static IEventManager* GetInstance();
};

class EventManager : public IEventManager
{
public:

    explicit EventManager(const char* pName, bool setAsGlobal);
    virtual ~EventManager() {}

    virtual bool VAddListener(
        const EventListenerDelegate& eventDelegate,
        const EventType& type
    );
    virtual bool VRemoveListener(
        const EventListenerDelegate& eventDelegate,
        const EventType& type
    );
    virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const;
    virtual bool VQueueEvent(const IEventDataPtr& pEvent);
    virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent);
    virtual bool VAbortEvent(const EventType& type, bool allOfType = false);
    virtual bool VTickUpdate(unsigned long maxMillis = kINFINITE);
private:

    typedef std::list<EventListenerDelegate> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::list<IEventDataPtr> EventQueue;

    EventListenerMap m_eventListeners;
    static const size_t EVTMGR_NUM_QUEUES = 2;
    EventQueue m_queues[EVTMGR_NUM_QUEUES];
    int m_activeQueue; // index of actively processing queue
    ThreadSafeEventQueue m_realtimeEventQueue;
};

#endif // GCC4_EVENT_MANAGER_H_INCLUDED

