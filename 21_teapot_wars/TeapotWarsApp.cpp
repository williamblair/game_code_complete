#include <GCC4/EventFactory.h>
#include "TeapotWarsApp.h"
#include "TeapotWarsLogic.h"
#include "TeapotEvents.h"
#include "MainMenuView.h"

BaseGameLogic* TeapotWarsApp::VCreateGameAndView()
{
    m_pGame = new TeapotWarsLogic();
    m_pGame->Init();

    std::shared_ptr<IGameView> menuView(new MainMenuView());
    m_pGame->VAddView(menuView);

    return m_pGame;
}

void TeapotWarsApp::VRegisterGameEvents()
{
    REGISTER_EVENT(EvtDataStartThrust);
    REGISTER_EVENT(EvtDataEndThrust);
    REGISTER_EVENT(EvtDataStartSteer);
    REGISTER_EVENT(EvtDataEndSteer);
}

void TeapotWarsApp::VCreateNetworkEventForwarder()
{
    GameCodeApp::VCreateNetworkEventForwarder();
    if (m_pNetworkEventForwarder != nullptr) {
        IEventManager* pGlobalEventManager = IEventManager::GetInstance();
        
        // future work - events should have a "classification" that signals if
        // they are sent from client to server, from server to client, or both.
        // Then as events are created, they are automatically added to the right
        // network forwarders.
        m_FireWeaponDelegate.reset(
            DECL_MBR_DELEGATE_THIS(
                &NetworkEventForwarder::ForwardEvent,
                m_pNetworkEventForwarder
            )
        );
        m_StartThrustDelegate.reset(
            DECL_MBR_DELEGATE_THIS(
                &NetworkEventForwarder::ForwardEvent,
                m_pNetworkEventForwarder
            )
        );
        m_EndThrustDelegate.reset(
            DECL_MBR_DELEGATE_THIS(
                &NetworkEventForwarder::ForwardEvent,
                m_pNetworkEventForwarder
            )
        );
        m_StartSteerDelegate.reset(
            DECL_MBR_DELEGATE_THIS(
                &NetworkEventForwarder::ForwardEvent,
                m_pNetworkEventForwarder
            )
        );
        m_EndSteerDelegate.reset(
            DECL_MBR_DELEGATE_THIS(
                &NetworkEventForwarder::ForwardEvent,
                m_pNetworkEventForwarder
            )
        );
        pGlobalEventManager->VAddListener(m_FireWeaponDelegate,EvtDataFireWeapon::sk_EventType);
        pGlobalEventManager->VAddListener(m_StartSteerDelegate,EvtDataStartSteer::sk_EventType);
        pGlobalEventManager->VAddListener(m_EndSteerDelegate,EvtDataEndSteer::sk_EventType);
        pGlobalEventManager->VAddListener(m_StartThrustDelegate,EvtDataStartThrust::sk_EventType);
        pGlobalEventManager->VAddListener(m_EndThrustDelegate,EvtDataEndThrust::sk_EventType);
    }
}

void TeapotWarsApp::VDestroyNetworkEventForwarder()
{
    GameCodeApp::VDestroyNetworkEventForwarder();
    if (m_pNetworkEventForwarder) {
        IEventManager* pGlobalEventManager = IEventManager::GetInstance();
        pGlobalEventManager->VRemoveListener(
            m_FireWeaponDelegate,
            EvtDataFireWeapon::sk_EventType
        );
        pGlobalEventManager->VRemoveListener(
            m_StartThrustDelegate,
            EvtDataStartThrust::sk_EventType
        );
        pGlobalEventManager->VRemoveListener(
            m_EndThrustDelegate,
            EvtDataEndThrust::sk_EventType
        );
        pGlobalEventManager->VRemoveListener(
            m_StartSteerDelegate,
            EvtDataStartSteer::sk_EventType
        );
        pGlobalEventManager->VRemoveListener(
            m_EndSteerDelegate,
            EvtDataEndSteer::sk_EventType
        );
        pGlobalEventManager->VRemoveListener(
            m_EnvironmentLoadedDelegate,
            EvtDataEnvironmentLoaded::sk_EventType
        );
    }
}

