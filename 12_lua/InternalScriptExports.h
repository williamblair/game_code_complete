#ifndef INTERNAL_SCRIPT_EXPORTS_H_INCLUDED
#define INTERNAL_SCRIPT_EXPORTS_H_INCLUDED

#include <memory>

#include <LuaStateManager.h>
#include <EventManager.h>
#include <EventData.h>
#include <ScriptEvent.h>

class InternalScriptExports
{
public:

    static bool Init();
    static bool Destroy();

    void AttachScriptProcess( LuaPlus::LuaObject scriptProcess );

    // these are exported to Lua
    static bool LoadAndExecuteScriptResource( const char* scriptResource );
    static bool QueueEvent( EventType eventType,
                            LuaPlus::LuaObject eventData );
    static bool TriggerEvent( EventType eventType,
                              LuaPlus::LuaObject eventData );

    static std::shared_ptr<ScriptEvent> BuildEvent( EventType type,
                                                    LuaPlus::LuaObject eventData );
};

namespace ScriptExports
{
    void Register();
    void Unregister();
}

#endif // INTERNAL_SCRIPT_EXPORTS_H_INCLUDED

