#include <GCC4/Logger.h>
#include "TeapotEvents.h"
#include "TeapotWarsHumanView.h"

TeapotWarsHumanView::TeapotWarsHumanView(std::shared_ptr<IRenderer> pRenderer) :
    HumanView(pRenderer)
{
    m_bShowUI = true;
    RegisterAllDelegates();
}
TeapotWarsHumanView::~TeapotWarsHumanView()
{
    RemoveAllDelegates();
}


bool TeapotWarsHumanView::VOnMsgProc(AppMsg msg)
{
    if (HumanView::VOnMsgProc(msg)) {
        return true;
    }

    if (msg.type == SDL_KEYDOWN) {
        //if (key == F1) {
        //    m_bShowUI = !m_bShowUI;
        //}
        
        // TODO
        /*if (key == F2) {
            POINT ptCursor;
            GetCursorPos(&ptCursor);
            ScreenToClient(g_pApp->GetHwnd(), &ptCursor);

            RayCast rayCast(ptCursor);
            m_pScene->Pick(&rayCast);
            rayCast.Sort();

            if (rayCast.m_NumIntersections) {
                // you can iterate through the intersections on the raycast
                int a = 0;
            }
        }*/

        //TODO
        /*if (key == F3) {
            extern void testThreading(ProcessManager* procMgr, bool runProtected);
            testThreading(m_pProcessManager, true);

            return true;

            Sleep(5000);
            testThreading(m_pProcessManager, false);
            Sleep(5000);

            extern void testRealtimeEvents(ProcessManager* procMgr);
            testRealtimeEvents(m_pProcessManager);

            extern void testRealtimeDecompression(ProcessManager* procMgr);
            testRealtimeDecompression(m_pProcessManager);
        }*/

        //TODO
        /*if (key == F4) {
            Resource resource("scripts/test.lua");
            std::shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);
        }*/

        /*if (key == F5) {
            std::shared_ptr<EvtDataScriptEventTestToLua> pEvent(
                new EvtDataScriptEventTestToLua
            );
            IEventManager::GetInstance()->VQueueEvent(pEvent);
        }*/

        //TODO
        /*if (key == F6) {
            LuaStateManager::GetInstance()->VExecuteString("TestProcess()");
        }*/

        //TODO
        /*if (key == F8) {
            TeapotWarsLogic* twg = static_cast<TeapotWarsLogic*>(g_pApp->m_pGame);
            twg->ToggleRenderDiagnostics();
        }*/

        //TODO
        /*if (key == F9) {
            m_KeyboardHandler = m_pTeapotController;
            m_PointerHandler = m_pTeapotController;
            m_pCamera->SetTarget(m_pTeapot);
            m_pTeapot->SetAlpha(0.8f);
            //ReleaseCapture();
            return true;
        }*/

        //TODO
        /*if (key == F11) {
            m_KeyboardHandler = m_pFreeCameraController;
            m_PointerHandler = m_pFreeCameraController;
            m_pCamera->ClearTarget();
            //SetCapture(g_pApp->GetHwnd());
            return true;
        }*/

        if (msg.key.keysym.sym == SDLK_q || msg.key.keysym.sym == SDLK_ESCAPE) { // 27 == escape
            //if (MessageBox::Ask(QUESTION_QUIT_GAME) == IDYES) {
                g_pApp->SetQuitting(true);
            //}
            return true;
        }
    }

    return false;
}

void TeapotWarsHumanView::VRenderText()
{
    //TODO
}
void TeapotWarsHumanView::VOnUpdate(unsigned long deltaMs)
{
    HumanView::VOnUpdate(deltaMs);
    if (m_pFreeCameraController) {
        m_pFreeCameraController->OnUpdate(deltaMs);
    }
    if (m_pTeapotController) {
        m_pTeapotController->OnUpdate(deltaMs);
    }

    //TODO
    // send out tick to listeners
    //std::shared_ptr<EvtDataUpdateTick> pTickEvent(new EvtDataUpdateTick(deltaMs));
    //IEventManager::GetInstance()->VTriggerEvent(pTickEvent);
}
void TeapotWarsHumanView::VOnAttach(GameViewId vid, ActorId aid)
{
    HumanView::VOnAttach(vid, aid);
}

void TeapotWarsHumanView::VSetControlledActor(ActorId actorId)
{
    m_pTeapot = std::static_pointer_cast<SceneNode>(m_pScene->FindActor(actorId));
    if (!m_pTeapot) {
        GCC_ERROR("Invalid teapot");
        return;
    }

    HumanView::VSetControlledActor(actorId);

    m_pTeapotController.reset(new TeapotController(m_pTeapot));
    m_KeyboardHandler = m_pTeapotController;
    m_PointerHandler = m_pTeapotController;
    m_pCamera->SetTarget(m_pTeapot);
    m_pTeapot->SetAlpha(0.8f);
}

bool TeapotWarsHumanView::VLoadGameDelegate(tinyxml2::XMLElement* pLevelData)
{
    if (!HumanView::VLoadGameDelegate(pLevelData)) {
        return false;
    }

    m_StandardHUD.reset(new StandardHUD);
    VPushElement(m_StandardHUD);

    // a movement controller is going to control the camera,
    // but it could be constructed with any of the objects you see in this
    // function. you can have your very own remote controlled sphere. what fun...
    m_pFreeCameraController.reset(new MovementController(m_pCamera, 0, 0, false));

    //m_pScene->VOnRestore();
    return true;
}

// event delegates
void TeapotWarsHumanView::GameplayUiUpdateDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataGameplayUIUpdate> pCastEventData =
        std::static_pointer_cast<EvtDataGameplayUIUpdate>(pEventData);
    if (!pCastEventData->GetGameplayUiString().empty()) {
        //m_gameplayText = s2ws(pCastEventData->GetUiString());
        m_gameplayText = pCastEventData->GetGameplayUiString();
    } else {
        m_gameplayText.clear();
    }
}
void TeapotWarsHumanView::SetControlledActorDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataSetControlledActor> pCastEventData =
        std::static_pointer_cast<EvtDataSetControlledActor>(pEventData);
    VSetControlledActor(pCastEventData->GetActorId());
}


void TeapotWarsHumanView::RegisterAllDelegates()
{
    IEventManager* pGlobalEventManager = IEventManager::GetInstance();
    m_GameplayUiUpdateDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsHumanView::GameplayUiUpdateDelegate)
    );
    m_SetControlledActorDelegate.reset(
        DECL_MBR_DELEGATE(&TeapotWarsHumanView::SetControlledActorDelegate)
    );
    pGlobalEventManager->VAddListener(m_GameplayUiUpdateDelegate, EvtDataGameplayUIUpdate::sk_EventType);
    pGlobalEventManager->VAddListener(m_SetControlledActorDelegate, EvtDataSetControlledActor::sk_EventType);
}
void TeapotWarsHumanView::RemoveAllDelegates()
{
    IEventManager* pGlobalEventManager = IEventManager::GetInstance();
    pGlobalEventManager->VRemoveListener(m_GameplayUiUpdateDelegate,EvtDataGameplayUIUpdate::sk_EventType);
    pGlobalEventManager->VRemoveListener(m_SetControlledActorDelegate,EvtDataSetControlledActor::sk_EventType);
}

