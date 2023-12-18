#include "MainMenuView.h"

MainMenuView::MainMenuView() :
    HumanView(std::shared_ptr<IRenderer>())
{
    m_MainMenuUI.reset(new MainMenuUI);
    VPushElement(m_MainMenuUI);
}
MainMenuView::~MainMenuView()
{
}

bool MainMenuView::VOnMsgProc(AppMsg msg)
{
    if (m_MainMenuUI->VIsVisible()) {
        if (!HumanView::VOnMsgProc(msg)) {
            return false;
        }
    }
    return true;
}

void MainMenuView::VRenderText()
{
    HumanView::VRenderText();
}

void MainMenuView::VOnUpdate(unsigned long deltaMs)
{
    HumanView::VOnUpdate(deltaMs);
}

