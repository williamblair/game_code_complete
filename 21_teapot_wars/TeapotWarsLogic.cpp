#include <GCC4/Actor.h>
#include <GCC4/EventManager.h>
#include <GCC4/NetworkGameView.h>
#include "AITeapotView.h"
#include "TeapotWarsHumanView.h"
#include "TeapotWarsLogic.h"

TeapotWarsLogic::TeapotWarsLogic()
{
    //TODO
}
TeapotWarsLogic::~TeapotWarsLogic()
{
    //TODO
}

// Update
void TeapotWarsLogic::VSetProxy()
{
    //TODO
}
void TeapotWarsLogic::VMoveActor(const ActorId id, Mat4x4 const& mat)
{
    //TODO
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
    //TODO
}


void TeapotWarsLogic::RequestStartGameDelegate(IEventDataPtr pEventData)
{
    //TODO
}

void TeapotWarsLogic::GameStateDelegate(IEventDataPtr pEventData)
{
    //TODO
}

void TeapotWarsLogic::RemoteClientDelegate(IEventDataPtr pEventData)
{
    //TODO
}

void TeapotWarsLogic::NetworkPlayerActorAssignmentDelegate(IEventDataPtr pEventData)
{
    //TODO
}

void TeapotWarsLogic::NewGameDelegate(IEventDataPtr pEventData)
{
    //TODO
}

bool TeapotWarsLogic::VLoadGameDelegate(tinyxml2::XMLElement* pLevelData)
{
    //TODO
    return true;
}

void TeapotWarsLogic::RegisterAllDelegates()
{
    //TODO
}

void TeapotWarsLogic::RemoveAllDelegates()
{
    //TODO
}

void TeapotWarsLogic::CreateNetworkEventForwarder(const int socketId)
{
    //TODO
}

void TeapotWarsLogic::DestroyAllNetworkEventForwarders()
{
    //TODO
}

