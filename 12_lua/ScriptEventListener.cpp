#include "ScriptEventListener.h"
#include <GCC4/EventManager.h>
#include <cassert>

ScriptEventListener::ScriptEventListener(
        const EventType& eventType,
        const LuaPlus::LuaObject& scriptCallbackFunc) :
    m_eventType(eventType),
    m_scriptCallbackFunc(scriptCallbackFunc)
{
    assert(scriptCallbackFunc.IsFunction() && "script callback func must be a function");
}

ScriptEventListener::~ScriptEventListener()
{
    IEventManager* pEventMgr = IEventManager::GetInstance();
    if (pEventMgr) {
        pEventMgr->VRemoveListener(GetDelegate(), m_eventType);
    }
}

void ScriptEventListener::ScriptEventDelegate(IEventDataPtr pEventPtr)
{
    std::shared_ptr<ScriptEvent> pScriptEvent =
        std::static_pointer_cast<ScriptEvent>(pEventPtr);
    LuaPlus::LuaFunctionVoid callback(m_scriptCallbackFunc);
    std::cout << "Calling script event delegate callback" << std::endl;
    if (m_scriptCallbackFunc.IsFunction())
    {
        if (pScriptEvent->GetEventData().IsInteger())
        {
            std::cout << "  is integer: " << pScriptEvent->GetEventData().GetInteger()
                << std::endl;
        }
        else if (pScriptEvent->GetEventData().IsNil())
        {
            std::cout << "  is nil" << std::endl;
        }
        callback(pScriptEvent->GetEventData());
    }
    else
    {
        std::cout << "ERROR - callback is not function" << std::endl;
        if (m_scriptCallbackFunc.IsNil())
        {
            std::cout << "  callback is nil" << std::endl;
        }
    }
}

