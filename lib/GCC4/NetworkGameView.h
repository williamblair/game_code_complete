#ifndef GCC4_NETWORK_GAME_VIEW_H_INCLUDED
#define GCC4_NETWORK_GAME_VIEW_H_INCLUDED

#include "IGameView.h"
#include "EventData.h"

class NetworkGameView : public IGameView
{
public:
    
    NetworkGameView();
    
    // IGameView implementation
    //virtual HRESULT VOnRestore() { return S_OK; }
    virtual void VOnRender(float fTime, float fElapsedTime) {}
    //virtual void VOnLostDevice() {}
    virtual GameViewType VGetType() { return GameView_Remote; }
    virtual GameViewId VGetId() const { return m_ViewId; }
    virtual void VOnAttach(GameViewId vid, ActorId aid) {
        m_ViewId = vid; m_ActorId = aid;
    }
    virtual bool VOnMsgProc(AppMsg msg) { return 0; }
    virtual void VOnUpdate(unsigned long deltaMilliseconds);

    void NewActorDelegate(IEventDataPtr pEventData);

    void SetPlayerActorId(ActorId actorId) { m_ActorId = actorId; }
    void AttachRemotePlayer(int sockId);
    int HasRemotePlayerAttached() { return m_SockId != -1; }

protected:
    GameViewId m_ViewId;
    ActorId m_ActorId;
    int m_SockId;
    EventListenerDelegate m_NewActorDelegate;
};

#endif // GCC4_NETWORK_GAME_VIEW_H_INCLUDED

