#include <EventManager.h>
#include <iostream>

EventManager::EventManager( const char* pName, bool setAsGlobal ) :
  IEventManager( pName, setAsGlobal )
{
    m_activeQueue = 0;
}

bool EventManager::VAddListener( const EventListenerDelegate& eventDelegate,
                                 const EventType& type )
{
    EventListenerList& eventListenerList = m_eventListeners[type];
    for ( auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it )
    {
        if ( eventDelegate == (*it) )
        {
            std::cout << __FILE__ << ":" << __LINE__ 
                      << ": attempting to double register a delegate" << std::endl;
            return false;
        }
    }

    eventListenerList.push_back( eventDelegate );
    return true;
}

bool EventManager::VRemoveListener( const EventListenerDelegate& eventDelegate,
                                    const EventType& type )
{
    bool success = false;

    auto findIt = m_eventListeners.find( type );
    if ( findIt != m_eventListeners.end() )
    {
        EventListenerList& listeners = findIt->second;
        for ( auto it = listeners.begin(); it != listeners.end(); ++it )
        {
            if ( eventDelegate == (*it) )
            {
                listeners.erase( it );
                success = true;

                // should be impossible for any more entries sense we can't
                // register the same delegate twice
                break;
            }
        }
    }

    return success;
}

bool EventManager::VTriggerEvent( const IEventDataPtr& pEvent ) const
{
    bool processed = false;

    auto findIt = m_eventListeners.find( pEvent->VGetEventType() );
    if ( findIt != m_eventListeners.end() )
    {
        const EventListenerList& eventListenerList = findIt->second;
        for ( EventListenerList::const_iterator it = eventListenerList.begin();
              it != eventListenerList.end();
              ++it )
        {
            EventListenerDelegate listener = *it;
            listener( pEvent );
            processed = true;
        }
    }

    return processed;
}




