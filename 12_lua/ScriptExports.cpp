#include <iostream>
#include <cassert>
#include <memory>
#include <LuaPlus/LuaPlus.h>
#include <GCC4/Process.h>
#include <GCC4/Resource.h>
#include <GCC4/ResCache.h>
#include "ScriptExports.h"
#include "App.h"

ScriptEventListenerMgr* InternalScriptExports::s_pScriptEventListenerMgr = nullptr;

bool InternalScriptExports::Init()
{
    if (s_pScriptEventListenerMgr == nullptr) {
        s_pScriptEventListenerMgr = new ScriptEventListenerMgr;
        if (!s_pScriptEventListenerMgr) { 
            std::cout << __FILE__ << ":" << __LINE__ << ": "
                << "Failed to allocate script event listener manager"
                << std::endl;
            return false;
        }
    }

    return true;
}

bool InternalScriptExports::Destroy()
{
    if (s_pScriptEventListenerMgr) {
        delete s_pScriptEventListenerMgr;
    }
    return true;
}

unsigned long
InternalScriptExports::RegisterEventListener(
    EventType eventType,
    LuaPlus::LuaObject callbackFunction)
{
    assert(s_pScriptEventListenerMgr);

    if (callbackFunction.IsFunction())
    {
        ScriptEventListener* pListener = new ScriptEventListener(
            eventType,
            callbackFunction
        );
        // save the data for later use
        s_pScriptEventListenerMgr->AddListener(pListener);
        IEventManager::GetInstance()->VAddListener(
            pListener->GetDelegate(),
            eventType
        );
        
        std::cout << "CPP added listener delegate" << std::endl;

        // convert the pointer to use as a handle
        unsigned long handle = reinterpret_cast<unsigned long>(pListener);
        return handle;
    }

    std::cout << __FILE__ << ":" << __LINE__ << ": "
        << "Attempting to register script event listener while manager null"
        << std::endl;

    return 0;
}

void InternalScriptExports::AttachScriptProcess( 
    LuaPlus::LuaObject scriptProcess)
{
    LuaPlus::LuaObject tmp = scriptProcess.Lookup("__object");
    if (!tmp.IsNil())
    {
        std::shared_ptr<Process> pProcess(
            static_cast<Process*>(tmp.GetLightUserdata())
        );
        g_pTestApp->m_pProcessMgr->AttachProcess(pProcess);
    }
    else
    {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "failed to find __object in scriptProcess" << std::endl;
    }
}

bool 
InternalScriptExports::LoadAndExecuteScriptResource(
    const char* scriptResource)
{
    Resource resource(scriptResource);
    std::shared_ptr<ResHandle> pResourceHandle =
        g_pTestApp->m_pResCache->GetHandle(&resource);
    if (pResourceHandle) {
        return true;
    }
    return false;
}

bool InternalScriptExports::QueueEvent(
    EventType type,
    LuaPlus::LuaObject eventData)
{
    std::shared_ptr<ScriptEvent> pEvent(BuildEvent(type, eventData));
    if (pEvent) {
        return IEventManager::GetInstance()->VQueueEvent(pEvent);
        //return true;
    }
    return false;
}

bool InternalScriptExports::TriggerEvent(
    EventType type,
    LuaPlus::LuaObject eventData)
{
    std::shared_ptr<ScriptEvent> pEvent(BuildEvent(type, eventData));
    if (pEvent) {
        return IEventManager::GetInstance()->VTriggerEvent(pEvent);
    }
    return false;
}


std::shared_ptr<ScriptEvent> InternalScriptExports::BuildEvent(
    EventType type,
    LuaPlus::LuaObject eventData)
{
    std::shared_ptr<ScriptEvent> pEvent(ScriptEvent::CreateEventFromScript(type));
    if (!pEvent) {
        return std::shared_ptr<ScriptEvent>();
    }
 
    // set the data that was passed in
    if (!pEvent->SetEventData(eventData)) {
        return std::shared_ptr<ScriptEvent>();
    }

    return pEvent;
}

namespace ScriptExports
{

void Register()
{
    assert(LuaStateManager::GetInstance());
    LuaPlus::LuaObject globals = LuaStateManager::GetInstance()->GetGlobalVars();

    if (!InternalScriptExports::Init()) {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "InternalScriptExports init failed" << std::endl;
        return;
    }

    globals.RegisterDirect("LoadAndExecuteScriptResource", &InternalScriptExports::LoadAndExecuteScriptResource);

    // event system
    globals.RegisterDirect("RegisterEventListener", &InternalScriptExports::RegisterEventListener);
    //globals.RegisterDirect( "RemoveEventListener", &InternalScriptExports::RemoveEventListener );
    globals.RegisterDirect("QueueEvent", &InternalScriptExports::QueueEvent);
    globals.RegisterDirect("TriggerEvent", &InternalScriptExports::TriggerEvent);
}

void Unregister()
{
    InternalScriptExports::Destroy();
}

} // end namespace ScriptExports



