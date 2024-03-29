#include <iostream>

#include <GCC4/LuaStateManager.h>
#include <GCC4/ScriptExports.h>

#include "App.h"
#include "EvtDataScriptEventTest.h"

#define RESOURCE_FILE "resources.zip"

LuaTestApp::LuaTestApp() :
    //m_pProcessMgr(nullptr),
    m_pResCache(nullptr),
    m_pZipFile(nullptr),
    m_pEvtMgr(nullptr),
    m_fromLuaTestEvtDelegate(
        DECL_MBR_DELEGATE(&LuaTestApp::FromLuaTestHandler)
    )
{
}

LuaTestApp::~LuaTestApp()
{
    if (m_pResCache != nullptr) {
        delete m_pResCache;
    }
    if (m_pProcessMgr != nullptr) {
        delete m_pProcessMgr;
    }

    IEventManager* pEvtMgr = IEventManager::GetInstance();
    pEvtMgr->VRemoveListener(
        m_fromLuaTestEvtDelegate,
        EvtDataScriptEventFromLua::sk_EventType
    );

    ScriptExports::Unregister();
    LuaStateManager::Destroy();
}

bool LuaTestApp::Init(const char* title, int screenWidth, int screenHeight)
{
    (void)title;
    (void)screenWidth;
    (void)screenHeight;

    // Resources
    m_pZipFile = new ResourceZipFile(RESOURCE_FILE); // freed automatically
    m_pResCache = new ResCache(50, m_pZipFile);
    if (!m_pResCache->Init()) {
        std::cout << __FILE__ << ":" << __LINE__
                  << ": failed to open " << RESOURCE_FILE << std::endl;
        return false;
    }

    // Processes
    m_pProcessMgr = new ProcessManager();
    if (!LuaStateManager::Create()) {
        return false;
    }

    // Events
    m_pEvtMgr = std::unique_ptr<EventManager>(
        new EventManager("EventManager", true) // set this as global instance
    ); 

    // Lua scripting
    ScriptExports::Register();

    REGISTER_SCRIPT_EVENT(
        EvtDataScriptEventToLua,
        EvtDataScriptEventToLua::sk_EventType
    );
    REGISTER_SCRIPT_EVENT(
        EvtDataScriptEventFromLua,
        EvtDataScriptEventFromLua::sk_EventType
    );
    // Load the Lua test event handler
    LuaStateManager* pLuaMgr = LuaStateManager::GetInstance();
    assert(pLuaMgr);
    pLuaMgr->VExecuteFile("scripts/EvtDataScriptEvent.lua");

    // Event handlers
    IEventManager* pEvtMgr = IEventManager::GetInstance();
    assert(pEvtMgr);
    pEvtMgr->VAddListener(
        m_fromLuaTestEvtDelegate,
        EvtDataScriptEventFromLua::sk_EventType
    );
    
    return true;
}

void LuaTestApp::Run()
{
    LuaStateManager* pLuaMgr = LuaStateManager::GetInstance();

    // Test basic Lua loading and running
    pLuaMgr->VExecuteString("print(\"Hello Lua\")" );
    pLuaMgr->VExecuteFile("test.lua");

    // Test Lua event handling
    std::shared_ptr<EvtDataScriptEventToLua> pEvent(
        new EvtDataScriptEventToLua()
    );
    IEventManager::GetInstance()->VQueueEvent(pEvent);

    // Let the events update
    for (size_t i = 0; i < 10; ++i) {
        IEventManager::GetInstance()->VTickUpdate();
    }
}

// Called when EvtDataScriptEventFromLua event happens;
// event is created by Lua Script scripts/EvtDataScriptEvent.lua
void LuaTestApp::FromLuaTestHandler(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataScriptEventFromLua> pCastEventData(
        std::static_pointer_cast<EvtDataScriptEventFromLua>(pEventData)
    );

    std::cout << "Received test event from Lua, value: "
        << pCastEventData->GetVal() << std::endl;
}

BaseGameLogic* LuaTestApp::VCreateGameAndView()
{
    return nullptr;
}

