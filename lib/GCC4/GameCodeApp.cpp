#include "GameCodeApp.h"
#include "ResourceZipFile.h"
#include "LuaStateManager.h"
#include "ScriptExports.h"
#include "ScriptProcess.h"
#include "OGLRenderer.h"

#define SCRIPT_PREINIT_FILE "Scripts\\PreInit.lua"

GameCodeApp* g_pApp = nullptr;

GameCodeApp::GameCodeApp() :
    m_pGame(nullptr),
    m_pResCache(nullptr),
    m_pEventManager(nullptr),
    m_pProcessMgr(nullptr),
    m_pBaseSocketManager(nullptr),
    m_pNetworkEventForwarder(nullptr)
{
}

bool GameCodeApp::Init(
    const char* title,
    int screenWidth,
    int screenHeight)
{
    RegisterEngineEvents();
    //VRegsiterGameEvents();
    
    IResourceFile* zipFile = new ResourceZipFile("Assets.zip");
    m_pResCache = new ResCache(50, zipFile);
    g_pResCache = m_pResCache;
    if (!m_pResCache->Init()) {
        printf("Failed to initialize resource cache!\n");
        return false;
    }

    // TODO
    //extern std::shared_ptr<IResourceLoader> CreateWAVResourceLoader();
    //extern std::shared_ptr<IResourceLoader> CreateOGGResourceLoader();
    //extern std::shared_ptr<IResourceLoader> CreateDDSResourceLoader();
    //extern std::shared_ptr<IResourceLoader> CreateJPGResourceLoader();
    //extern std::shared_ptr<IResourceLoader> CreateXmlResourceLoader();
    //extern std::shared_ptr<IResourceLoader> CreateSdkMeshResourceLoader();
    extern std::shared_ptr<IResourceLoader> CreateScriptResourceLoader();

    // Register these in order from least to most specific
    //m_pResCache->RegisterLoader(CreateWAVResourceLoader());
    //m_pResCache->RegisterLoader(CreateOGGResourceLoader());
    //m_pResCache->RegisterLoader(CreateDDSResourceLoader());
    //m_pResCache->RegisterLoader(CreateJPGResourceLoader());
    //m_pResCache->RegisterLoader(CreateXmlResourceLoader());
    //m_pResCache->RegisterLoader(CreateSdkMeshResourceLoader());
    m_pResCache->RegisterLoader(CreateScriptResourceLoader());

    //if (!LoadStrings("English"))
    //{
    //  printf("Failed to load strings\n");
    //  return false;
    //}

    if (!LuaStateManager::Create()) {
        printf("Failed to initialize Lua\n");
        return false;
    }

    // Load the preinit file.
    // This is within braces to create a scope and destroy the resource once it's loaded.
    // We don't need to do anything with it, we just need to load it.
    {
        // this actually loads the XML file from the zip file
        Resource resource(SCRIPT_PREINIT_FILE);
        std::shared_ptr<ResHandle> pResourceHandle = m_pResCache->GetHandle(&resource);
    }

    // Register function exported from C++
    ScriptExports::Register();
    ScriptProcess::RegisterScriptClass();
    //TODO
    //BaseScriptComponent::RegisterScriptFunctions();

    // The event manager should be created next so that subsystems can hook in as desired.
    m_pEventManager = new EventManager("GameCodApp Event Mgr", true);
    if (!m_pEventManager) {
        printf("Failed to create Event Manager\n");
        return false;
    }

    if (GetRendererImpl() == Renderer_OGL) {
        m_Renderer = std::shared_ptr<IRenderer>(new OGLRenderer);
    } else {
        printf("Unsupported Renderer impl\n");
        return false;
    }
    m_Renderer->VSetBackgroundColor(255, 20, 20, 200);
    m_Renderer->VOnRestore();

    //TODO
    //m_pGame = VCreateGameAndView();
    //if (!m_pGame) {
    //    printf("Failed to create game and view\n");
    //    return false;
    //}

    // now that all major systems are initialized, preload resources
    m_pResCache->Preload("*.ogg", nullptr);
    m_pResCache->Preload("*.dds", nullptr);
    m_pResCache->Preload("*.jpg", nullptr);

    //TODO
    //if (GameCodeApp::GetRendererImpl() == GameCodeApp::Renderer_D3D11) {
    //  m_pResCache->Preload("*.sdkmesh", nullptr);
    //}

    //TODO
    //CheckForJoystick();
    
    m_bIsRunning = true;
    return true;
}

bool GameCodeApp::OnDisplayChange(int colorDepth, int width, int height)
{
    // TODO
    return true;
}

bool GameCodeApp::OnClose()
{
    delete m_pGame;
    
    VDestroyNetworkEventForwarder();
    
    delete m_pBaseSocketManager;
    delete m_pEventManager;
    
    //TODO
    //BaseScriptComponent::UnregisterScriptFunctions();
    ScriptExports::Unregister();
    LuaStateManager::Destroy();

    delete m_pResCache;
    g_pResCache = nullptr;

    return true;
}

GameCodeApp::Renderer GameCodeApp::GetRendererImpl()
{
    return Renderer_OGL;
}

void GameCodeApp::OnUpdateGame(double fTime, float fElapsedTime, void* pUserContext)
{
    (void)pUserContext;

    //TODO
    //if (g_pApp->HasModalDialog()) {
        // don't update the game if a modal dialog is up
        //return;
    //}
    if (g_pApp->m_bQuitting) {
        //TODO
        //PostMessage(g_pApp->GetHwnd(), WM_CLOSE, 0,0);
    }

    if (g_pApp->m_pGame) {
        // allow event queue to process for up to 20 ms
        //IEventManager::GetInstance()->VUpdate(20);
        IEventManager::GetInstance()->VTickUpdate(20);

        if (g_pApp->m_pBaseSocketManager) {
            g_pApp->m_pBaseSocketManager->DoSelect(0); // pause 0 microseconds
        }

        g_pApp->m_pGame->VOnUpdate(float(fTime), fElapsedTime);
    }
}

bool GameCodeApp::VLoadGame()
{
    // Read game options and see what the current game needs to be -
    // all of the game graphics are initialized by now too
    return m_pGame->VLoadGame(m_Options.m_Level.c_str());
}

void GameCodeApp::VCreateNetworkEventForwarder()
{
    if (m_pNetworkEventForwarder != nullptr) {
        printf("Overwriting network event forwarder in TeapotWarsApp!\n");
        delete m_pNetworkEventForwarder;
    }
    m_pNetworkEventForwarder = new NetworkEventForwarder(0);

    IEventManager* pGlobalEventManager = IEventManager::GetInstance();
    m_NetRequestNewActorDelegate.reset(
        DECL_MBR_DELEGATE_THIS(
            &NetworkEventForwarder::ForwardEvent,
            m_pNetworkEventForwarder
        )
    );
    pGlobalEventManager->VAddListener(
        m_NetRequestNewActorDelegate,
        EvtDataRequestNewActor::sk_EventType
    );
    m_NetEnvironmentLoadedDelegate.reset(
        DECL_MBR_DELEGATE_THIS(
            &NetworkEventForwarder::ForwardEvent,
            m_pNetworkEventForwarder
        )
    );
    pGlobalEventManager->VAddListener(
        m_NetEnvironmentLoadedDelegate,
        EvtDataEnvironmentLoaded::sk_EventType
    );
    m_NetPhysCollisionDelegate.reset(
        DECL_MBR_DELEGATE_THIS(
            &NetworkEventForwarder::ForwardEvent,
            m_pNetworkEventForwarder
        )
    );
    pGlobalEventManager->VAddListener(
        m_NetPhysCollisionDelegate,
        EvtDataPhysCollision::sk_EventType
    );
}

void GameCodeApp::VDestroyNetworkEventForwarder()
{
    if (!m_pNetworkEventForwarder) {
        return;
    }

    IEventManager* pGlobalEventManager = IEventManager::GetInstance();
    pGlobalEventManager->VRemoveListener(
        m_NetRequestNewActorDelegate,
        EvtDataRequestNewActor::sk_EventType
    );
    pGlobalEventManager->VRemoveListener(
        m_NetEnvironmentLoadedDelegate,
        EvtDataEnvironmentLoaded::sk_EventType
    );
    pGlobalEventManager->VRemoveListener(
        m_NetPhysCollisionDelegate,
        EvtDataPhysCollision::sk_EventType
    );

    delete m_pNetworkEventForwarder;
}

#define REGISTER_EVENT(eventClass) g_eventFactory.Register<eventClass>(eventClass::sk_EventType)
void GameCodeApp::RegisterEngineEvents()
{
    // TODO - g_eventFactory
    /*
    REGISTER_EVENT(EvtDataEnvironmentLoaded);
    REGISTER_EVENT(EvtDataNewActor);
    REGISTER_EVENT(EvtDataMoveActor);
    REGISTER_EVENT(EvtDataDestroyActor);
    REGISTER_EVENT(EvtDataRequestNewActor);
    REGISTER_EVENT(EvtDataNetworkPlayerActorAssignment);
    */
}


