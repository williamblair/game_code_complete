#include <iostream>
#include <App.h>
#include <LuaStateManager.h>

App* g_pApp = nullptr;

#define RESOURCE_FILE "resources.zip"

App::App() :
    m_pProcessMgr( nullptr ),
    m_pResCache( nullptr ),
    m_pZipFile( nullptr )
{
}

App::~App()
{
    if ( m_pResCache != nullptr ) {
        delete m_pResCache;
    }
    if ( m_pProcessMgr != nullptr ) {
        delete m_pProcessMgr;
    }

    LuaStateManager::Destroy();
}

bool App::Init()
{
    m_pZipFile = new ResourceZipFile( RESOURCE_FILE ); // freed automatically
    m_pResCache = new ResCache( 50, m_pZipFile );
    if ( !m_pResCache->Init() ) {
        std::cout << __FILE__ << ":" << __LINE__
                  << ": failed to open " << RESOURCE_FILE << std::endl;
        return false;
    }
    m_pProcessMgr = new ProcessManager();

    if ( !LuaStateManager::Create() ) {
        return false;
    }
    return true;
}

void App::Run()
{
    LuaStateManager* pLuaMgr = LuaStateManager::GetInstance();

    pLuaMgr->VExecuteString( "print(\"Hello Lua\")" );
    pLuaMgr->VExecuteFile( "test.lua" );
}

