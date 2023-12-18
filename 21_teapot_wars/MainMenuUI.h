#ifndef MAIN_MENU_UI_H_INCLUDED
#define MAIN_MENU_UI_H_INCLUDED

#include <string>
#include <vector>

#include <GCC4/BaseUI.h>

class MainMenuUI : public BaseUI
{
public:

    MainMenuUI();
    virtual ~MainMenuUI();

    // IScreenElement implementation
    virtual bool VOnRestore();
    virtual bool VOnRender(float fTime, float fElapsedTime);
    virtual int VGetZOrder() const { return 1; }
    virtual void VSetZOrder(int const zOrder) {}

    virtual bool VOnMsgProc(AppMsg msg);
    // TODO
    //static void OnGUIEvent(unsigned int nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
    //void _OnGUIEvent(unsigned int nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);

protected:
    //TODO
    //CDXUTDialog m_SampleUI;
    
    void Set();
    bool m_bCreatingGame;
    int m_NumAIs;
    int m_NumPlayers;
    std::vector<std::string> m_Levels;
    int m_LevelIndex;
    std::string m_HostName;
    std::string m_HostListenPort;
    std::string m_ClientAttachPort;
};

#endif // MAIN_MENU_UI_H_INCLUDED

