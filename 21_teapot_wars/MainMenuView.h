#ifndef MAIN_MENU_VIEW_H_INCLUDED
#define MAIN_MENU_VIEW_H_INCLUDED

#include <GCC4/HumanView.h>

class MainMenuView : public HumanView
{
public:
    MainMenuView();
    ~MainMenuView();

    virtual bool VOnMsgProc(AppMsg msg);
    virtual void VRenderText();
    virtual void VOnUpdate(unsigned long deltaMs);

protected:
    std::shared_ptr<MainMenuUI> m_MainMenuUI;
};

#endif // MAIN_MENU_VIEW_H_INCLUDED

