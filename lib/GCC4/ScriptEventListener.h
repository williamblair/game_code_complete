#ifndef GCC4_SCRIPT_EVENT_LISTENER_H_INCLUDED
#define GCC4_SCRIPT_EVENT_LISTENER_H_INCLUDED

#include <LuaPlus/LuaPlus.h>

#include "EventData.h"
//#include "ScriptEvent.h"

class ScriptEventListener
{
public:

    explicit ScriptEventListener(
        const EventType& eventType,
        const LuaPlus::LuaObject& scriptCallbackFunc
    );
    ~ScriptEventListener();

    EventListenerDelegate GetDelegate()
    {
        return EventListenerDelegate( 
            DECL_MBR_DELEGATE(&ScriptEventListener::ScriptEventDelegate)
        );
    }

    void ScriptEventDelegate(IEventDataPtr pEventPtr);

private:

    EventType m_eventType;
    LuaPlus::LuaObject m_scriptCallbackFunc;

};

#endif // GCC4_SCRIPT_EVENT_LISTENER_H_INCLUDED

