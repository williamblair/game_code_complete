#ifndef GCC4_NETWORK_GAME_VIEW_H_INCLUDED
#define GCC4_NETWORK_GAME_VIEW_H_INCLUDED

class NetworkGameView : public IGameView
{
public:
    // IGameView implementation
    virtual HRESULT VOnRestore() { return S_OK; }
    virtual void VOnRender(double fTime, float fElapsedTime) {}
    virtual void VOnLostDevice() {}
    virtual GameViewType VGetType() { return GameView_Remote; }
    virtual GameViewId VGetId() const { return m_ViewId; }
    virtual void VOnAttach(GameViewId vid, ActorId aid) {
        m_ViewId = vid; m_PlayerActorId = aid;
    }
    virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) { return 0; }
    virtual void VOnUpdate(int deltaMilliseconds) {}

    void SetPlayerActorId(ActorId actorId) { m_ActorId = actorId; }
    void AttachRemotePlayer(int sockId);
    int HasRemotePlayerAttached() { return m_SockId != -1; }

    NetworkGameView(int sockId);

protected:
    GameViewId m_ViewId;
    ActorId m_ActorId;
    int m_SockId;
};

#endif // GCC4_NETWORK_GAME_VIEW_H_INCLUDED

