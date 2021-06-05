#include <EvtDataScriptEventTest.h>
#include <LuaStateManager.h>

const EventType EvtDataScriptEventFromLua::sk_EventType = EventType(0x12345678);
const EventType EvtDataScriptEventToLua::sk_EventType = EventType(0x12345679);

EvtDataScriptEventFromLua::EvtDataScriptEventFromLua()
{
}

EvtDataScriptEventToLua::EvtDataScriptEventToLua()
{
}

void EvtDataScriptEventToLua::VBuildEventData()
{
    std::cout << "Calling build event data, setting to 5" << std::endl;
    LuaStateManager* luaMgr = LuaStateManager::GetInstance();
    m_num = 5;
    m_eventData.AssignNumber( luaMgr->GetLuaState(), m_num );
}

bool EvtDataScriptEventFromLua::VBuildEventFromScript()
{
    if ( m_eventData.IsInteger() )
    {
        m_num = m_eventData.GetInteger();
        return true;
    }

    return false;
}

