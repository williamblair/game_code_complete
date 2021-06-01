#ifndef INTERNAL_SCRIPT_EXPORTS_H_INCLUDED
#define INTERNAL_SCRIPT_EXPORTS_H_INCLUDED

#include <LuaStateManager.h>

class InternalScriptExports
{
public:

    static bool Init();
    static bool Destroy();

    void AttachScriptProcess( LuaPlus::LuaObject scriptProcess );

    // these are exported to Lua
    static bool LoadAndExecuteScriptResource( const char* scriptResource );
};

namespace ScriptExports
{
    void Register();
    void Unregister();
}

#endif // INTERNAL_SCRIPT_EXPORTS_H_INCLUDED

