#ifndef TEAPOT_WARS_APP_H_INCLUDED
#define TEAPOT_WARS_APP_H_INCLUDED

#include <GCC4/GameCodeApp.h>

class TeapotWarsApp : public GameCodeApp
{
public:

    virtual const char* VGetGameTitle() { return "Teapot Wars"; }
    virtual const char* VGetGameAppDirectory() { return "./"; }

private:
    virtual BaseGameLogic* VCreateGameAndView();

    virtual void VRegisterGameEvents();
    virtual void VCreateNetworkEventForwarder();
    virtual void VDestroyNetworkEventForwarder();

    EventListenerDelegate m_FireWeaponDelegate;
    EventListenerDelegate m_StartThrustDelegate;
    EventListenerDelegate m_EndThrustDelegate;
    EventListenerDelegate m_StartSteerDelegate;
    EventListenerDelegate m_EndSteerDelegate;
    EventListenerDelegate m_EnvironmentLoadedDelegate;
};

#endif // TEAPOT_WARS_APP_H_INCLUDED

