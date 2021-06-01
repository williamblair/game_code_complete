#include <iostream>
#include <cassert>
#include <ScriptEvent.h>
#include <LuaStateManager.h>

ScriptEvent::CreationFunctions ScriptEvent::s_creationFunctions;

ScriptEvent::ScriptEvent() :
    m_eventDataIsValid( false )
{
}

void ScriptEvent::RegisterEventTypeWithScript( const char* key, EventType type )
{
    LuaPlus::LuaObject eventTypeTable = 
        LuaStateManager::GetInstance()->GetGlobalVars().GetByName( "EventType" );
    if ( eventTypeTable.IsNil() ) {
        eventTypeTable = LuaStateManager::GetInstance()->GetGlobalVars()
            .CreateTable( "EventType" );
    }

    assert( eventTypeTable.IsTable() );
    assert( eventTypeTable[key].IsNil() );

    eventTypeTable.SetNumber( key, (double)type );
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

