#include <GCC4/Actor.h>
#include <GCC4/EventManager.h>
#include <GCC4/NetworkGameView.h>
#include <GCC4/MakeStrongPtr.h>
#include <GCC4/TransformComponent.h>
#include <GCC4/PhysicsComponent.h>
#include "AITeapotView.h"
#include "TeapotWarsHumanView.h"
#include "TeapotEvents.h"
#include "TeapotWarsLogic.h"

TeapotWarsLogic::TeapotWarsLogic()
{
    //TODO
    //m_pPhysics.reset(CreateGamePhysics());
    RegisterAllDelegates();
}
TeapotWarsLogic::~TeapotWarsLogic()
{
    RemoveAllDelegates();
    DestroyAllNetworkEventForwarders();
}

// Update
void TeapotWarsLogic::VSetProxy()
{
    // FUTURE WORK: this can go in base game logic
    BaseGameLogic::VSetProxy();
}
void TeapotWarsLogic::VMoveActor(const ActorId id, Mat4x4 const& mat)
{
    BaseGameLogic::VMoveActor(id, mat);

    // FUTURE WORK: this would make a great basis for a trigger actor that ran a
    // LUA script when other actors entered or left it
    
    StrongActorPtr pActor = MakeStrongPtr(VGetActor(id));
    if (pActor) {
        std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(
            pActor->GetComponent<TransformComponent>(TransformComponent::g_Name)
        );
        if (pTransformComponent && pTransformComponent->GetPosition().y < -25) {
            std::shared_ptr<EvtDataDestroyActor> pDestroyActorEvent(
                new EvtDataDestroyActor(id)
            );
            IEventManager::GetInstance()->VQueueEvent(pDestroyActorEvent);
        }
    }
}

// Overloads
void TeapotWarsLogic::VChangeState(BaseGameState newState)
{
    BaseGameLogic::VChangeState(newState);

    switch (newState)
    {
    case BGS_WaitingForPlayers:
    {
        // spawn all local players (should only be one)
        assert(m_ExpectedPlayers == 1);
        for (int i=0; i<m_ExpectedPlayers; ++i) {
            std::shared_ptr<IGameView> playersView(
                new TeapotWarsHumanView(g_pApp->m_Renderer)
            );
            VAddView(playersView);

            if (m_bProxy) {
                // if we are a remote player, all we have to do is spawn
                // our view - the server will do the rest
                return;
            }
        }

        // spawn all remote players views on the game
        for (int i=0; i<m_ExpectedRemotePlayers; ++i) {
            std::shared_ptr<IGameView> remoteGameView(new NetworkGameView);
            VAddView(remoteGameView);
        }

        // spawn all AI views on the game
        for (int i=0; i<m_ExpectedAi; ++i) {
            std::shared_ptr<IGameView> aiView(new AITeapotView(m_pPathingGraph));
            VAddView(aiView);
        }

        break;
    }

    case BGS_SpawnPlayersActors:
    {
        if (m_bProxy) {
            // only the server needs to do this
            return;
        }

        for (auto it = m_GameViews.begin(); it != m_GameViews.end(); ++it)
        {
            std::shared_ptr<IGameView> pView = *it;
            if (pView->VGetType() == GameView_Human) {
                StrongActorPtr pActor = VCreateActor("actors\\player_teapot.xml", nullptr);
                if (pActor) {
                    std::shared_ptr<EvtDataNewActor> pNewActorEvent(
                        new EvtDataNewActor(
                            pActor->GetId(),
                            pView->VGetId()
                        )
                    );
                    IEventManager::GetInstance()->VTriggerEvent(pNewActorEvent);
                }
            }

            else if (pView->VGetType() == GameView_Remote) {
                std::shared_ptr<NetworkGameView> pNetworkGameView =
                    std::static_pointer_cast<NetworkGameView,IGameView>(pView);
                StrongActorPtr pActor = VCreateActor("actors\\remote_teapot.xml", nullptr);
                if (pActor) {
                    std::shared_ptr<EvtDataNewActor> pNewActorEvent(
                        new EvtDataNewActor(
                            pActor->GetId(),
                            pNetworkGameView->VGetId()
                        )
                    );
                    IEventManager::GetInstance()->VTriggerEvent(pNewActorEvent);
                }
            }

            else if (pView->VGetType() == GameView_AI) {
                std::shared_ptr<AITeapotView> pAiView =
                    std::static_pointer_cast<AITeapotView,IGameView>(pView);
                StrongActorPtr pActor = VCreateActor("actors\\ai_teapot.xml", nullptr);
                if (pActor) {
                    std::shared_ptr<EvtDataNewActor> pNewActorEvent(
                        new EvtDataNewActor(
                            pActor->GetId(),
                            pAiView->VGetId()
                        )
                    );
                    IEventManager::GetInstance()->VTriggerEvent(pNewActorEvent);
                }
            }
        }

        break;
    }

    default:
        break;
    }
}

void TeapotWarsLogic::VAddView(
    std::shared_ptr<IGameView> pView,
    ActorId actorId)
{
    BaseGameLogic::VAddView(pView, actorId);

    if (std::dynamic_pointer_cast<TeapotWarsHumanView>(pView)) {
        m_HumanPlayersAttached++;
    }
    else if (std::dynamic_pointer_cast<AITeapotView>(pView)) {
        m_AiPlayersAttached++;
    }
}


void TeapotWarsLogic::RequestStartGameDelegate(IEventDataPtr pEventData)
{
    (void)pEventData;
    VChangeState(BGS_WaitingForPlayers);
}

void TeapotWarsLogic::RemoteClientDelegate(IEventDataPtr pEventData)
{
    // This event is always sent from clients to the game server

    std::shared_ptr<EvtDataRemoteClient> pCastEventData =
        std::static_pointer_cast<EvtDataRemoteClient>(pEventData);
    const int sockID = pCastEventData->GetSocketId();
    const int ipAddress = pCastEventData->GetIpAddress();

    // go find a NetworkView that doesn't have a socket ID,
    // and attach this client to it
    for (auto it = m_GameViews.begin(); it != m_GameViews.end(); ++it) {
        std::shared_ptr<IGameView> pView = *it;
        if (pView->VGetType() == GameView_Remote) {
            std::shared_ptr<NetworkGameView> pNetworkGameView =
                std::static_pointer_cast<NetworkGameView,IGameView>(pView);
            if (!pNetworkGameView->HasRemotePlayerAttached()) {
                pNetworkGameView->AttachRemotePlayer(sockID);
                CreateNetworkEventForwarder(sockID);
                m_HumanPlayersAttached++;

                return;
            }
        }
    }
}

void TeapotWarsLogic::NetworkPlayerActorAssignmentDelegate(IEventDataPtr pEventData)
{
    if (!m_bProxy) {
        return;
    }

    // we're a remote client getting an actor assignment
    // the server assigned us a playerId when we first attached (the server's socketId, actually)
    std::shared_ptr<EvtDataNetworkPlayerActorAssignment> pCastEventData =
        std::static_pointer_cast<EvtDataNetworkPlayerActorAssignment>(pEventData);
    if (pCastEventData->GetActorId() == INVALID_ACTOR_ID) {
        m_RemotePlayerId = pCastEventData->GetSocketId();
        return;
    }

    for (auto it = m_GameViews.begin(); it != m_GameViews.end(); ++it) {
        std::shared_ptr<IGameView> pView = *it;
        if (pView->VGetType() == GameView_Human) {
            std::shared_ptr<TeapotWarsHumanView> pHumanView =
                std::static_pointer_cast<TeapotWarsHumanView,IGameView>(pView);
            if (m_RemotePlayerId == pCastEventData->GetSocketId()) {
                pHumanView->VSetControlledActor(pCastEventData->GetActorId());
            }
            return;
        }
    }

    GCC_ERROR("Could not find HumanView to attach actor to!");
}

void TeapotWarsLogic::EnvironmentLoadedDelegate(IEventDataPtr pEventData)
{
    (void)pEventData;
    ++m_HumanGamesLoaded;
}

void TeapotWarsLogic::StartThrustDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataStartThrust> pCastEventData =
        std::static_pointer_cast<EvtDataStartThrust>(pEventData);
    StrongActorPtr pActor = MakeStrongPtr(VGetActor(pCastEventData->GetActorId()));
    if (pActor) {
        std::shared_ptr<PhysicsComponent> pPhysicalComponent =
            MakeStrongPtr(
                pActor->GetComponent<PhysicsComponent>(
                    PhysicsComponent::g_Name
                )
            );
        if (pPhysicalComponent) {
            pPhysicalComponent->ApplyAcceleration(pCastEventData->GetAcceleration());
        }
    }
}

void TeapotWarsLogic::EndThrustDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataStartThrust> pCastEventData =
        std::static_pointer_cast<EvtDataStartThrust>(pEventData);
    StrongActorPtr pActor = MakeStrongPtr(VGetActor(pCastEventData->GetActorId()));
    if (pActor) {
        std::shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(
            pActor->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name)
        );
        if (pPhysicalComponent) {
            pPhysicalComponent->RemoveAcceleration();
        }
    }
}

void TeapotWarsLogic::StartSteerDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataStartThrust> pCastEventData =
        std::static_pointer_cast<EvtDataStartThrust>(pEventData);
    StrongActorPtr pActor = MakeStrongPtr(VGetActor(pCastEventData->GetActorId()));
    if (pActor) {
        std::shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(
            pActor->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name)
        );
        if (pPhysicalComponent) {
            pPhysicalComponent->ApplyAngularAcceleration(
                pCastEventData->GetAcceleration()
            );
        }
    }
}

void TeapotWarsLogic::EndSteerDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataStartThrust> pCastEventData =
        std::static_pointer_cast<EvtDataStartThrust>(pEventData);
    StrongActorPtr pActor = MakeStrongPtr(VGetActor(pCastEventData->GetActorId()));
    if (pActor) {
        std::shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(
            pActor->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name)
        );
        if (pPhysicalComponent) {
            pPhysicalComponent->RemoveAngularAcceleration();
        }
    }
}

void TeapotWarsLogic::TestScriptDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataScriptEventTestFromLua> pCastEventData =
        std::static_pointer_cast<EvtDataScriptEventTestFromLua>(pEventData);
    GCC_LOG("Lua", "Event received in C++ from Lua: " + std::to_string(pCastEventData->GetNum()));
}

bool TeapotWarsLogic::VLoadGameDelegate(tinyxml2::XMLElement* pLevelData)
{
    RegisterTeapotScriptEvents();
    return true;
}

void TeapotWarsLogic::RegisterAllDelegates()
{
    // FUTURE WORK: Lots of these functions are ok to go into the base game logic!

    m_RemoteClientDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::RemoteClientDelegate)
    );
    m_MoveActorDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::MoveActorDelegate)
    );
    m_RequestStartGameDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::RequestStartGameDelegate)
    );
    m_NetworkPlayerActorAssignmentDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::NetworkPlayerActorAssignmentDelegate)
    );
    m_EnvironmentLoadedDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::EnvironmentLoadedDelegate)
    );

    IEventManager* pGlobalEventManager = IEventManager::GetInstance();
    pGlobalEventManager->VAddListener(m_RemoteClientDelegate, EvtDataRemoteClient::sk_EventType);
    pGlobalEventManager->VAddListener(m_MoveActorDelegate, EvtDataMoveActor::sk_EventType);
    pGlobalEventManager->VAddListener(m_RequestStartGameDelegate, EvtDataRequestStartGame::sk_EventType);
    pGlobalEventManager->VAddListener(m_NetworkPlayerActorAssignmentDelegate, EvtDataNetworkPlayerActorAssignment::sk_EventType);
    pGlobalEventManager->VAddListener(m_EnvironmentLoadedDelegate, EvtDataEnvironmentLoaded::sk_EventType);
    pGlobalEventManager->VAddListener(m_EnvironmentLoadedDelegate, EvtDataRemoteEnvironmentLoaded::sk_EventType);

    // FUTURE WORK: only these belong in TeapotWars.
    
    m_StartThrustDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::StartThrustDelegate)
    );
    m_EndThrustDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::EndThrustDelegate)
    );
    m_StartSteerDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::StartSteerDelegate)
    );
    m_EndSteerDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::EndSteerDelegate)
    );
    m_TestScriptDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsLogic::TestScriptDelegate)
    );

    pGlobalEventManager->VAddListener(m_StartThrustDelegate, EvtDataStartThrust::sk_EventType);
    pGlobalEventManager->VAddListener(m_EndThrustDelegate, EvtDataEndThrust::sk_EventType);
    pGlobalEventManager->VAddListener(m_StartSteerDelegate, EvtDataStartSteer::sk_EventType);
    pGlobalEventManager->VAddListener(m_EndSteerDelegate, EvtDataEndSteer::sk_EventType);

    pGlobalEventManager->VAddListener(m_TestScriptDelegate, EvtDataScriptEventTestFromLua::sk_EventType);
}

void TeapotWarsLogic::RemoveAllDelegates()
{
    IEventManager* pGlobalEventManager = IEventManager::GetInstance();
    pGlobalEventManager->VRemoveListener(m_RemoteClientDelegate, EvtDataRemoteClient::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_MoveActorDelegate, EvtDataMoveActor::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_RequestStartGameDelegate, EvtDataRequestStartGame::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_NetworkPlayerActorAssignmentDelegate, EvtDataNetworkPlayerActorAssignment::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_EnvironmentLoadedDelegate, EvtDataEnvironmentLoaded::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_EnvironmentLoadedDelegate, EvtDataRemoteEnvironmentLoaded::sk_EventType);
    //TODO
    //if (m_bProxy) {
    //    pGlobalEventManager->VRemoveListener(m_RequestNewActorDelegate, EvtDataRequestNewActor::sk_EventType);
    //}

    pGlobalEventManager->VRemoveListener(m_StartThrustDelegate, EvtDataStartThrust::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_EndThrustDelegate, EvtDataEndThrust::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_StartSteerDelegate, EvtDataStartSteer::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_EndSteerDelegate, EvtDataEndSteer::sk_EventType);

    pGlobalEventManager->VRemoveListener(m_TestScriptDelegate, EvtDataScriptEventTestFromLua::sk_EventType);
}

void TeapotWarsLogic::CreateNetworkEventForwarder(const int socketId)
{
    //TODO
}

void TeapotWarsLogic::DestroyAllNetworkEventForwarders()
{
    //TODO
}

