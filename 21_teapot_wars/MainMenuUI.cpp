#include <GCC4/GameCodeApp.h>
#include "MainMenuUI.h"


MainMenuUI::MainMenuUI()
{
    m_NumAIs = g_pApp->m_Options.m_NumAis;
    m_NumPlayers = g_pApp->m_Options.m_ExpectedPlayers;
    m_HostName = g_pApp->m_Options.m_GameHost;
    m_HostListenPort = g_pApp->m_Options.m_ListenPort;
    m_ClientAttachPort = g_pApp->m_Options.m_ListenPort;

    m_bCreatingGame = true;

    std::vector<Level> levels =
        g_pApp->GetGameLogic()->GetLevelManager()->GetLevels();
    m_Levels.reserve(levels.size());
    int count = 0;
    for (auto i = levels.begin(); i != levels.end(); ++i, ++count)
    {
        //m_Levels.push_back(s2ws(*i));
        m_Levels.push_back(*i);
    }

    Set();
}

void MainMenuUI::Set()
{
    //TODO
#if 0
    char ansiBuffer[256];
    m_LevelIndex = m_SampleUI.GetListBox(CID_LEVEL_LISTBOX)->GetSelectedIndex();
    m_SampleUI.GetListBox(CID_LEVEL_LISTBOX)->SetVisible(m_bCreatingGame);

    m_NumAIs = m_SampleUI.GetSlider(CID_NUM_AI_SLIDER)->GetValue();
    m_SampleUI.GetSlider(CID_NUM_AI_SLIDER)->SetVisible(m_bCreatingGame);

    m_NumPlayers = m_SampleUI.GetSlider(CID_NUM_PLAYER_SLIDER)->GetValue();
    m_SampleUI.GetSlider(CID_NUM_PLAYER_SLIDER)->SetVisible(m_bCreatingGame);

    if (m_bCreatingGame) {
        WideToAnsiCch(ansiBuffer, m_SampleUI.GetEditBox(CID_HOST_LISTEN_PORT)->GetText(), 256);
        m_HostListenPort = ansiBuffer;
    }

    WideToAnsiCch(ansiBuffer, m_SampleUI.GetEditBox(CID_HOST_NAME)->GetText(), 256);
    m_HostName = ansiBuffer;

    if (!m_bCreatingGame) {
        WideToAnsiCch(ansiBuffer, m_SampleUI.GetEditBox(CID_CLIENT_ATTACH_PORT)->GetText(), 256);
        m_ClientAttachPort = ansiBuffer;
    }
#endif
}

MainMenuUI::~MainMenuUI()
{
}

bool MainMenuUI::VOnRestore()
{
    //TODO
    return true;
}

bool MainMenuUI::VOnRender(float fTime, float fElapsedTime)
{
    //TODO
    //m_SampleUI.OnRender(fElapsedTime);
    return true;
}

bool MainMenuUI::VOnMsgProc(AppMsg msg)
{
    //TODO
    //return m_SampleUI.MsgProc(msg);
    return true;
}

/*void MainMenuUI::OnGUIEvent(unsigned int nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
    //TODO
}
void MainMenuUI::_OnGUIEvent(unsigned int nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
    //TODO
}*/

