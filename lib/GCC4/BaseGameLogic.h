#ifndef GCC4_BASE_GAME_LOGIC_H_INCLUDED
#define GCC4_BASE_GAME_LOGIC_H_INCLUDED

#include <map>
#include <vector>
#include <string>

#include "ComponentTypes.h"
#include "IGameLogic.h"
#include "IGamePhysics.h"
#include "IGameView.h"
#include "EventData.h"
#include "ProcessManager.h"
#include "GCCRandom.h"
#include "BaseGameState.h"
#include "PathingGraph.h"

// Forward declarations
class PathingGraph;
class ActorFactory;
class LevelManager;

typedef std::map<ActorId, StrongActorPtr> ActorMap;
typedef std::string Level;

class LevelManager
{
public:
    const std::vector<Level>& GetLevels() const { return m_Levels; }
    const int GetCurrentLevel() const { return m_CurrentLevel; }
    bool Initialize(const std::vector<std::string>& levels);
private:
    std::vector<Level> m_Levels;
    int m_CurrentLevel;
};

class BaseGameLogic : public IGameLogic
{
    friend class GameCodeApp; // to access the view list
public:

    BaseGameLogic();
    virtual ~BaseGameLogic();

    bool Init();

    void SetProxy(bool isProxy) { m_bProxy = isProxy; }
    const bool IsProxy() const { return m_bProxy; }

    const bool CanRunLua() const { return !IsProxy() || (GetState() != BGS_Running); }

    ActorId GetNewActorId() { return ++m_LastActorId; }

    std::shared_ptr<PathingGraph> GetPathingGraph() { return m_pPathingGraph; }

    GCCRandom& GetRNG() { return m_Random; }

    virtual void VAddView(std::shared_ptr<IGameView> pView, ActorId actorId = INVALID_ACTOR_ID);
    virtual void VRemoveView(std::shared_ptr<IGameView> pView);

    virtual StrongActorPtr VCreateActor(
        const std::string& actorResource,
        tinyxml2::XMLElement* overrides,
        const Mat4x4* initialTransform = nullptr,
        const ActorId serversActorId = INVALID_ACTOR_ID
    );
    virtual void VDestroyActor(const ActorId actorId);
    virtual WeakActorPtr VGetActor(const ActorId actorId);
    virtual void VModifyActor(const ActorId actorId, tinyxml2::XMLElement* overrides);
    virtual void VMoveActor(const ActorId id, const Mat4x4& mat) {}

    // editor functions
    std::string GetActorXml(const ActorId id);

    // level management
    const LevelManager* GetLevelManager() { return  m_pLevelManager; }
    virtual bool VLoadGame(const char* levelResource) override; // subclasses shouldn't override this function; use VLoadGameDelegate instead
    virtual void VSetProxy();

    // logic update
    virtual void VOnUpdate(float time, float elapsedTime);

    // changing game logic state
    virtual void VChangeState(BaseGameState newState);
    const BaseGameState GetState() const { return m_State; }

    // Render diagnostics
    void ToggleRenderDiagnostics() { m_bRenderDiagnostics = !m_bRenderDiagnostics; }
    virtual void VRenderDiagnostics();
    virtual std::shared_ptr<IGamePhysics> VGetGamePhysics() { return m_pPhysics; }

    void AttachProcess(StrongProcessPtr pProcess) {
        if (m_pProcessManager) {
            m_pProcessManager->AttachProcess(pProcess);
        }
    }

    // event delegates
    void RequestDestroyActorDelegate(IEventDataPtr pEventData);

protected:

    float m_LifeTime; // indicates how long this game has been in session
    ProcessManager* m_pProcessManager;
    GCCRandom m_Random;
    ActorMap m_Actors;
    ActorId m_LastActorId;
    BaseGameState m_State; // game stating: loading, running, etc...
    int m_ExpectedPlayers; // how many local human players
    int m_ExpectedRemotePlayers; // expected remote human players
    int m_ExpectedAi; // how many AI players
    int m_HumanPlayersAttached;
    int m_AiPlayersAttached;
    int m_HumanGamesLoaded;
    GameViewList m_GameViews;
    std::shared_ptr<PathingGraph> m_pPathingGraph; // the pathing graph
    ActorFactory* m_pActorFactory;

    bool m_bProxy; // set if this is a proxy game logic, not a real one
    int m_RemotePlayerId; // if we are a remote player - what is out socker number on the server

    bool m_bRenderDiagnostics; // are we rendering diagnostics?
    std::shared_ptr<IGamePhysics> m_pPhysics;

    LevelManager* m_pLevelManager; // manages loading and chaining levels

    virtual ActorFactory* VCreateActorFactory();

    // override this function to do any game-specific loading.
    virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData) { return true; }

    void MoveActorDelegate(IEventDataPtr pEventData);
    void RequestNewActorDelegate(IEventDataPtr pEventData);
    EventListenerDelegate m_MoveActorDelegate;
    EventListenerDelegate m_RequestNewActorDelegate;
};

#endif // GCC4_BASE_GAME_LOGIC_H_INCLUDED

