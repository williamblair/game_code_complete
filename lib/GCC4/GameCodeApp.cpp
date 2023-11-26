#include "GameCodeApp.h"

GameCodeApp::GameCodeApp()
{
    //TODO
}

bool GameCodeApp::Init(
    const char* title,
    int screenWidth,
    int screenHeight)
{
    //TODO
    return true;
}

bool GameCodeApp::OnDisplayChange(int colorDepth, int width, int height)
{
    //TODO
    return true;
}

GameCodeApp::Renderer GameCodeApp::GetRendererImpl()
{
    return Renderer_OGL;
}

void GameCodeApp::OnUpdateGame(double fTime, float fElapsedTime, void* pUserContext)
{
    //TODO
}

bool GameCodeApp::VLoadGame()
{
    //TODO
    return true;
}

void GameCodeApp::VCreateNetworkEventForwarder()
{
    //TODO
}
void GameCodeApp::VDestroyNetworkEventForwarder()
{
    //TODO
}

void GameCodeApp::RegisterEngineEvents()
{
    //TODO
}


