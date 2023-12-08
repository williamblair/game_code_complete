#include "TeapotWarsApp.h"
#include "TeapotWarsLogic.h"

TeapotWarsApp g_TeapotWarsApp;

int GameCode4(int argc, char* argv[])
{
    //TODO
    //Logger::Init("logging.xml");
    
    g_pApp->m_Options.Init("PlayerOptions.xml", argc, argv);
    
    //TODO
    //DXUTSetCallbackMsgProc(GameCodeApp::MsgProc)
    //DXUTSetCallbackFrameMove(GameCodeApp::OnUpdateGame)
    //DXUTSetCAllbackDeviceChanging(GameCodeApp::ModifyDeviceSettings)
    
    //TODO
    /*if (g_pApp->m_Options.m_Renderer == "Direct3D 9")
    {
        DXUTSetCallbackD3D9DeviceAcceptable( GameCodeApp::IsD3D9DeviceAcceptable );
        DXUTSetCallbackD3D9DeviceCreated( GameCodeApp::OnD3D9CreateDevice );
        DXUTSetCallbackD3D9DeviceReset( GameCodeApp::OnD3D9ResetDevice );
        DXUTSetCallbackD3D9DeviceLost( GameCodeApp::OnD3D9LostDevice );
        DXUTSetCallbackD3D9DeviceDestroyed( GameCodeApp::OnD3D9DestroyDevice );
        DXUTSetCallbackD3D9FrameRender( GameCodeApp::OnD3D9FrameRender );
    }
    else if (g_pApp->m_Options.m_Renderer == "Direct3D 11")
    {
        DXUTSetCallbackD3D11DeviceAcceptable( GameCodeApp::IsD3D11DeviceAcceptable );
        DXUTSetCallbackD3D11DeviceCreated( GameCodeApp::OnD3D11CreateDevice );
        DXUTSetCallbackD3D11SwapChainResized( GameCodeApp::OnD3D11ResizedSwapChain );
        DXUTSetCallbackD3D11SwapChainReleasing( GameCodeApp::OnD3D11ReleasingSwapChain );
        DXUTSetCallbackD3D11DeviceDestroyed( GameCodeApp::OnD3D11DestroyDevice );
        DXUTSetCallbackD3D11FrameRender( GameCodeApp::OnD3D11FrameRender );
    }
    else
    {
        printf("Unknown renderer specified in game options\n");
        return 1;
    }*/
    
    if (!g_pApp->Init("Teapot Wars", 640, 480)) {
        return 1;
    }
    
    //TODO
    //DXUTMainLoop();
    //DXUTShutdown();
    
    //TODO
    //Logger::Destroy();
    
    return g_pApp->GetExitCode();
}

#ifdef _WIN32
#undef main
#endif

int main(int argc, char* argv[])
{
    return GameCode4(argc, argv);
}

