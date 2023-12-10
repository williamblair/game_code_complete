#ifndef TEAPOT_WARS_LOGIC_H_INCLUDED
#define TEAPOT_WARS_LOGIC_H_INCLUDED

#include <list>

#include <GCC4/BaseGameLogic.h>
#include <GCC4/NetSocket.h>

class TeapotWarsLogic : public BaseGameLogic
{
public:

    TeapotWarsLogic();
    virtual ~TeapotWarsLogic();

    // Update
    virtual void VSetProxy();
    virtual void VMoveActor(const ActorId id, Mat4x4 const& mat);

    // Overloads
    virtual void VChangeState(BaseGameState newState);
    virtual void VAddView(
        std::shared_ptr<IGameView> pView,
        ActorId actorId = INVALID_ACTOR_ID
    );
    virtual std::shared_ptr<IGamePhysics> VGetGamePhysics() {
        return m_pPhysics;
    }

    // set/clear render diagnostics
    void ToggleRenderDiagnostics() { m_bRenderDiagnostics = !m_bRenderDiagnostics; }

    // event delegates
    void RequestStartGameDelegate(IEventDataPtr pEventData);
    void GameStateDelegate(IEventDataPtr pEventData);
    void RemoteClientDelegate(IEventDataPtr pEventData);
    void NetworkPlayerActorAssignmentDelegate(IEventDataPtr pEventData);
    void NewGameDelegate(IEventDataPtr pEventData);

private:
    std::list<NetworkEventForwarder*> m_NetworkEventForwarders;

    virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData);

    void RegisterAllDelegates();
    void RemoveAllDelegates();
    void CreateNetworkEventForwarder(const int socketId);
    void DestroyAllNetworkEventForwarders();
};

#endif // TEAPOT_WARS_LOGIC_H_INCLUDED

