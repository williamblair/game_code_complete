#ifndef STANDARD_HUD_H_INCLUDED
#define STANDARD_HUD_H_INCLUDED

#include <GCC4/BaseUI.h>

class StandardHUD : public BaseUI
{
public:
    StandardHUD();
    virtual ~StandardHUD();

    // IScreenElement implementation
    virtual bool VOnRestore();
    virtual bool VOnRender(float fTime, float fElapsedTime);
    virtual int VGetZOrder() const { return 1; }
    virtual void VSetZOrder(int const zOrder) {}

    virtual bool VOnMsgProc(AppMsg msg);
    //TODO
    //static void OnGUIEvent(unsigned int nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);

protected:
    //TODO
    //CDXUTDialog m_HUD;
};

#endif // STANDARD_HUD_H_INCLUDED

