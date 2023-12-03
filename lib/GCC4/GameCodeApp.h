#ifndef GCC4_GAME_CODE_APP_H_INCLUDED
#define GCC4_GAME_CODE_APP_H_INCLUDED

#include <memory>

class HumanView;

#include "EventManager.h"
#include "ResCache.h"
#include "IRenderer.h"
#include "BaseGameLogic.h"
#include "GameOptions.h"
#include "HumanView.h"
#include "SocketManager.h"
#include "NetSocket.h"

#ifndef _MAX_PATH
// arbitrarily chosen
#define _MAX_PATH 1024
#endif

class GameCodeApp
{
public:
    
    GameCodeApp();

    // Game Application Data
    // You must define these in an inherited class - see TeapotWarsApp for example
    virtual const char* VGetGameTitle() = 0;
    virtual const char* VGetGameAppDirectory() = 0;

    virtual bool Init(const char* title, int screenWidth, int screenHeight);

    bool OnDisplayChange(int colorDepth, int width, int height);
    bool OnClose();

    enum Renderer
    {
        Renderer_Unknown,
        Renderer_OGL
    };
    std::shared_ptr<IRenderer> m_Renderer;
    static Renderer GetRendererImpl();

    static void OnUpdateGame(double fTime, float fElapsedTime, void* pUserContext);

    BaseGameLogic* m_pGame;
    GameOptions m_Options;
    BaseGameLogic* GetGameLogic() { return m_pGame; }

    // You must define these functions to initialize your game.
    // Look in TeapotWars.h and TeapotWars.cpp
    virtual BaseGameLogic* VCreateGameAndView() = 0;
    virtual bool VLoadGame();
    HumanView* GetHumanView();

    // File and Resource system
    ResCache* m_pResCache;
    char m_saveGameDirectory[_MAX_PATH];

    bool IsEditorRunning() { return m_bIsEditorRunning; }

    // Event manager
    EventManager* m_pEventManager;

    // Process manager
    ProcessManager* m_pProcessMgr;

    // Socker manager - could be server or client
    BaseSocketManager* m_pBaseSocketManager;
    NetworkEventForwarder* m_pNetworkEventForwarder;
    bool AttachAsClient();

    // Main loop processing
    void AbortGame() { m_bQuitting = true; }
    int GetExitCode() { return 0; /* TODO */ }
    bool IsRunning() { return m_bIsRunning; }
    void SetQuitting(bool quitting) { m_bQuitting = quitting; }

protected:
    bool m_bWindowedMode; // true if the app is windowed, false if fullscreen
    bool m_bIsRunning; // true if everything is initialized and the game is in the main loop
    bool m_bQuitRequested; // true if the app should run the exit sequence
    bool m_bIsEditorRunning; // true if the game editor is running
    bool m_bQuitting;

    virtual void VRegisterGameEvents() {}

    virtual void VCreateNetworkEventForwarder();
    virtual void VDestroyNetworkEventForwarder();

    EventListenerDelegate m_NetRequestNewActorDelegate;
    EventListenerDelegate m_NetEnvironmentLoadedDelegate;
    EventListenerDelegate m_NetPhysCollisionDelegate;

private:
    void RegisterEngineEvents();
};

extern GameCodeApp* g_pApp; // GameCodeApp.cpp

#endif // GCC4_GAME_CODE_APP_H_INCLUDED

