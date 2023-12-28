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
    void RemoteClientDelegate(IEventDataPtr pEventData);
    void NetworkPlayerActorAssignmentDelegate(IEventDataPtr pEventData);
    void EnvironmentLoadedDelegate(IEventDataPtr pEventData);
    //void ThrustDelegate(IEventDataPtr pEventData);
    //void SteerDelegate(IEventDataPtr pEventData);
    void StartThrustDelegate(IEventDataPtr pEventData);
    void EndThrustDelegate(IEventDataPtr pEventData);
    void StartSteerDelegate(IEventDataPtr pEventData);
    void EndSteerDelegate(IEventDataPtr pEventData);

    void TestScriptDelegate(IEventDataPtr pEventData);

private:
    std::list<NetworkEventForwarder*> m_NetworkEventForwarders;

    EventListenerDelegate m_RequestStartGameDelegate;
    EventListenerDelegate m_RemoteClientDelegate;
    EventListenerDelegate m_NetworkPlayerActorAssignmentDelegate;
    EventListenerDelegate m_EnvironmentLoadedDelegate;
    EventListenerDelegate m_StartThrustDelegate;
    EventListenerDelegate m_EndThrustDelegate;
    EventListenerDelegate m_StartSteerDelegate;
    EventListenerDelegate m_EndSteerDelegate;

    EventListenerDelegate m_TestScriptDelegate;

    virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData);

    void RegisterAllDelegates();
    void RemoveAllDelegates();
    void CreateNetworkEventForwarder(const int socketId);
    void DestroyAllNetworkEventForwarders();
};

#endif // TEAPOT_WARS_LOGIC_H_INCLUDED

