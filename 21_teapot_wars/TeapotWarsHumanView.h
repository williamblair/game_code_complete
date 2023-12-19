#ifndef TEAPOT_WARS_HUMAN_VIEW_H_INCLUDED
#define TEAPOT_WARS_HUMAN_VIEW_H_INCLUDED

#include <GCC4/HumanView.h>
#include <GCC4/MovementController.h>
#include "StandardHUD.h"
#include "TeapotController.h"

class TeapotWarsHumanView : public HumanView
{
public:

    TeapotWarsHumanView(std::shared_ptr<IRenderer> pRenderer);
    virtual ~TeapotWarsHumanView();

    virtual bool VOnMsgProc(AppMsg msg);
    virtual void VRenderText();
    virtual void VOnUpdate(unsigned long deltaMs);
    virtual void VOnAttach(GameViewId vid, ActorId aid);

    virtual void VSetControlledActor(ActorId actorId);
    virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData) override;

    // event delegates
    void GameplayUiUpdateDelegate(IEventDataPtr pEventData);
    void SetControlledActorDelegate(IEventDataPtr pEventData);

protected:
    bool m_bShowUI;
    std::string m_gameplayText;

    std::shared_ptr<TeapotController> m_pTeapotController;
    std::shared_ptr<MovementController> m_pFreeCameraController;
    std::shared_ptr<SceneNode> m_pTeapot;
    std::shared_ptr<StandardHUD> m_StandardHUD;

    EventListenerDelegate m_GameplayUiUpdateDelegate;
    EventListenerDelegate m_SetControlledActorDelegate;

private:
    void RegisterAllDelegates();
    void RemoveAllDelegates();
};

#endif // TEAPOT_WARS_HUMAN_VIEW_H_INCLUDED

