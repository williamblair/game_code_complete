#include <iostream>
#include <cassert>
#include <ScriptEvent.h>
#include <LuaStateManager.h>

ScriptEvent::CreationFunctions ScriptEvent::s_creationFunctions;

ScriptEvent::ScriptEvent() :
    m_eventDataIsValid( false )
{
}

ScriptEvent::~ScriptEvent()
{
}

LuaPlus::LuaObject ScriptEvent::GetEventData()
{
    if ( !m_eventDataIsValid )
    {
        VBuildEventData();
        m_eventDataIsValid = true;
    }
    return m_eventData;
}

bool ScriptEvent::SetEventData( LuaPlus::LuaObject eventData )
{
    m_eventData = eventData;
    m_eventDataIsValid = VBuildEventFromScript();
    return m_eventDataIsValid;
}

void ScriptEvent::RegisterEventTypeWithScript( const char* key, EventType type )
{
    LuaPlus::LuaObject eventTypeTable = 
        LuaStateManager::GetInstance()->GetGlobalVars().GetByName( "EventType" );
    if ( eventTypeTable.IsNil() ) {
        std::cout << "Creating event type table" << std::endl;
        eventTypeTable = LuaStateManager::GetInstance()->GetGlobalVars()
            .CreateTable( "EventType" );
    }

    assert( eventTypeTable.IsTable() );
    assert( eventTypeTable[key].IsNil() );

    std::cout << "Setting eventype number, key: " << key << ", type: "
              << (unsigned long long)type << std::endl;
    eventTypeTable.SetNumber( key, (unsigned long long)type );
}

void ScriptEvent::AddCreationFunction( EventType type, 
                                       CreateEventForScriptFunctionType pCreateFunc )
{
    if ( s_creationFunctions.find(type) != s_creationFunctions.end() ) {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
                  << "warning: overwriting existing create function of type "
                  << (unsigned long)type << std::endl;
    }
    s_creationFunctions[type] = pCreateFunc;
}

ScriptEvent* ScriptEvent::CreateEventFromScript( EventType type )
{
    auto it = s_creationFunctions.find( type );
    if ( it != s_creationFunctions.end() )
    {
        CreateEventForScriptFunctionType func = it->second;
        return func();
    }
    std::cout << __FILE__ << ":" << __LINE__ << ": "
              << "failed to find event type" << std::endl;
    return nullptr;
}

void ScriptEvent::VBuildEventData()
{
    LuaStateManager* luaMgr = LuaStateManager::GetInstance();
    m_eventData.AssignNil( luaMgr->GetLuaState() );
}

