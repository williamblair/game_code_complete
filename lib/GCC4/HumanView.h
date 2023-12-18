#ifndef GCC4_HUMAN_VIEW_H_INCLUDED
#define GCC4_HUMAN_VIEW_H_INCLUDED

#include "AppMsg.h"
#include "BaseGameState.h"
#include "CameraNode.h"
#include "EventData.h"
#include "GameCodeApp.h"
#include "GCCTime.h"
#include "GCCMath.h"
#include "IGameView.h"
#include "IKeyboardHandler.h"
#include "IPointerHandler.h"
#include "IRenderer.h"
#include "IScreenElement.h"
#include "ProcessManager.h"
#include "ScreenElementScene.h"
#include "xml.h"

class HumanView : public IGameView
{
    friend class GameCodeApp;

public:

    HumanView(std::shared_ptr<IRenderer> pRenderer);
    virtual ~HumanView();

    // IGameView interface
    virtual void VOnRender(float fTime, float fElapsedTime);
    virtual GameViewType VGetType() { return GameView_Human; }
    virtual GameViewId VGetId() const { return m_ViewId; }
    virtual void VOnAttach(GameViewId vid, ActorId aid) {
        m_ViewId = vid;
        m_ActorId = aid;
    }
    virtual bool VOnMsgProc(AppMsg msg);
    virtual void VOnUpdate(unsigned long deltaMs);

    bool LoadGame(tinyxml2::XMLElement* pLevelData);

    ProcessManager* GetProcessManager() { return m_pProcessManager; }

    // methods to control the layering of interface elements
    virtual void VPushElement(std::shared_ptr<IScreenElement> pElement);
    virtual void VRemoveElement(std::shared_ptr<IScreenElement> pElement);

    void TogglePause(bool bActive);

    // Interface sensitive objects
    std::shared_ptr<IPointerHandler> m_PointerHandler;
    int m_PointerRadius;
    std::shared_ptr<IKeyboardHandler> m_KeyboardHandler;

    // Audio
    bool InitAudio();

    // Camera adjustments
    virtual void VSetCameraOffset(const Vec4& camOffset);

    std::shared_ptr<ScreenElementScene> m_pScene;
    ScreenElementList m_ScreenElements;
    std::shared_ptr<CameraNode> m_pCamera;


    void HandleGameState(BaseGameState newState);

    // helps the network system attach views to the right actor
    virtual void VSetControllerActor(ActorId actorId) { m_ActorId = actorId; }

    // Event delegates
    EventListenerDelegate m_PlaySoundDelegate;
    EventListenerDelegate m_GameStateDelegate;
    void PlaySoundDelegate(IEventDataPtr pEventData);
    void GameStateDelegate(IEventDataPtr pEventData);

protected:
    GameViewId m_ViewId;
    ActorId m_ActorId;

    ProcessManager* m_pProcessManager;

    uint32_t m_currTick; // time right now
    uint32_t m_lastDraw; // last time the game rendered
    bool m_bRunFullSpeed; // true to run at full speed

    BaseGameState m_BaseGameState;

    virtual void VRenderText() {}
    virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData) {
        VPushElement(m_pScene);
        return true;
    }

private:
    void RegisterAllDelegates();
    void RemoveAllDelegates();
};

#endif // GCC4_HUMAN_VIEW_H_INCLUDED

