#ifndef GCC4_INTERNAL_SCRIPT_EXPORTS_H_INCLUDED
#define GCC4_INTERNAL_SCRIPT_EXPORTS_H_INCLUDED

#include <memory>

#include "EventManager.h"
#include "EventData.h"
#include "LuaStateManager.h"
//#include "ScriptEvent.h"
#include "ScriptEventListener.h"
#include "ScriptEventListenerMgr.h"

class InternalScriptExports
{
public:

    static bool Init();
    static bool Destroy();


    // these are exported to Lua
    static void AttachScriptProcess(LuaPlus::LuaObject scriptProcess);
    static unsigned long RegisterEventListener(
        EventType eventType,
        LuaPlus::LuaObject callbackFunction
    );
    static bool LoadAndExecuteScriptResource(const char* scriptResource);
    static bool QueueEvent(
        EventType eventType,
        LuaPlus::LuaObject eventData
    );
    static bool TriggerEvent(
        EventType eventType,
        LuaPlus::LuaObject eventData
    );

    static std::shared_ptr<ScriptEvent> BuildEvent(
        EventType type,
        LuaPlus::LuaObject eventData
    );

private:
    static ScriptEventListenerMgr* s_pScriptEventListenerMgr;
};

namespace ScriptExports
{
    void Register();
    void Unregister();
}

#endif // GCC4_INTERNAL_SCRIPT_EXPORTS_H_INCLUDED

