#include <iostream>
#include <memory>
#include <LuaPlus/LuaPlus.h>
#include <InternalScriptExports.h>
#include <Process.h>
#include <Resource.h>
#include <ResCache.h>
#include <App.h>

bool InternalScriptExports::Init()
{
    // TODO
    return false;
}

bool InternalScriptExports::Destroy()
{
    // TODO
    return false;
}

void InternalScriptExports::AttachScriptProcess( 
    LuaPlus::LuaObject scriptProcess )
{
    LuaPlus::LuaObject tmp = scriptProcess.Lookup( "__object" );
    if ( !tmp.IsNil() )
    {
        std::shared_ptr<Process> pProcess(
            static_cast<Process*>( tmp.GetLightUserdata() ) );
        g_pApp->m_pProcessMgr->AttachProcess( pProcess );
    }
    else
    {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
                  << "failed to find __object in scriptProcess" << std::endl;
    }
}

bool 
InternalScriptExports::LoadAndExecuteScriptResource(
    const char* scriptResource )
{
    Resource resource( scriptResource );
    std::shared_ptr<ResHandle> pResourceHandle =
        g_pApp->m_pResCache->GetHandle( &resource );
    if ( pResourceHandle ) {
        return true;
    }
    return false;
}

void ScriptExports::Register()
{
    LuaPlus::LuaObject globals = LuaStateManager::GetInstance()->GetGlobalVars();

    InternalScriptExports::Init();

    globals.RegisterDirect(
        "LoadAndExecuteScriptResource",
        InternalScriptExports::LoadAndExecuteScriptResource );
}

void ScriptExports::Unregister()
{
    InternalScriptExports::Destroy();
}

