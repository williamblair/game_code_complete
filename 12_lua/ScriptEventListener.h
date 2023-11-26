#ifndef SCRIPT_EVENT_LISTENER_H_INCLUDED
#define SCRIPT_EVENT_LISTENER_H_INCLUDED

#include <LuaPlus/LuaPlus.h>

#include <GCC4/EventData.h>
#include "ScriptEvent.h"

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

#endif // SCRIPT_EVENT_LISTENER_H_INCLUDED

