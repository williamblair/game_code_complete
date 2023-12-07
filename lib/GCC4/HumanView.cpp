#include <cassert>
#include "HumanView.h"
#include "EventManager.h"
#include "Resource.h"

#define SCREEN_REFRESH_RATE 1000/60

HumanView::HumanView(std::shared_ptr<IRenderer> pRenderer)
{
    InitAudio();

    m_pProcessManager = new ProcessManager;

    m_PointerRadius = 1;
    m_ViewId = gc_InvalidGameViewId;

    // for move, new, and destroy actor events and others
    RegisterAllDelegates();
    m_BaseGameState = BGS_Initializing;

    if (pRenderer) {
        m_pScene.reset(new ScreenElementScene(pRenderer));

        Frustum frustum;
        frustum.Init(M_PI/4.0f, 1.0f, 1.0f, 100.0f);
        m_pCamera.reset(new CameraNode(&Mat4x4::g_Identity, frustum));
        assert(m_pScene && m_pCamera);

        m_pScene->VAddChild(INVALID_ACTOR_ID, m_pCamera);
        m_pScene->SetCamera(m_pCamera);
    }
}
HumanView::~HumanView()
{
    RemoveAllDelegates();

    while (!m_ScreenElements.empty()) {
        m_ScreenElements.pop_front();
    }

    if (m_pProcessManager) { delete m_pProcessManager; }
    //TODO
    //if (g_pAudio) { delete g_pAudio; }
}

template<class T>
struct SortBy_SharedPtr_Content
{
    bool operator()(
            const std::shared_ptr<T>& lhs,
            const std::shared_ptr<T>& rhs) const {
        return *lhs < *rhs;
    }
};

void HumanView::VOnRender(float fTime, float fElapsedTime)
{
    m_currTick = GCC4Time::timeGetTime();
    if (m_currTick == m_lastDraw) {
        return;
    }

    if (m_bRunFullSpeed || ((m_currTick - m_lastDraw) > SCREEN_REFRESH_RATE))
    {
        if (g_pApp->m_Renderer->VPreRender())
        {
            m_ScreenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());
            for (auto i=m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i) {
                if ((*i)->VIsVisible()) {
                    (*i)->VOnRender(fTime, fElapsedTime);
                }
            }

            VRenderText();

            m_lastDraw = m_currTick;
        }

        g_pApp->m_Renderer->VPostRender();
    }
}

bool HumanView::VOnMsgProc(AppMsg msg)
{
    // Iterate through screen layers first
    // In reverse order since we'll send input messages to the
    // screen on top
    for (auto i = m_ScreenElements.rbegin(); i != m_ScreenElements.rend(); ++i)
    {
        if ((*i)->VIsVisible()) {
            if ((*i)->VOnMsgProc(msg)) {
                return true;
            }
        }
    }

    /*bool result = false;*/
    switch (msg.type)
    {
    case SDL_KEYDOWN:
        if (m_KeyboardHandler) {
            /*result = */m_KeyboardHandler->VOnKeyDown(AppMsgKeyToChar(msg));
        }
        break;
    case SDL_KEYUP:
        if (m_KeyboardHandler) {
            /*result = */m_KeyboardHandler->VOnKeyUp(AppMsgKeyToChar(msg));
        }
        break;
    case SDL_MOUSEMOTION:
        if (m_PointerHandler) {
            /*result = */m_PointerHandler->VOnPointerMove(
                Point(msg.motion.x,msg.motion.y),
                1 // radius
            );
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (m_PointerHandler) {
            /*result = */m_PointerHandler->VOnPointerButtonDown(
                Point(msg.button.x,msg.button.y),
                1,
                msg.button.button == 1 ? "PointerLeft" :
                    (msg.button.button == 3 ? "PointerRight" :
                        "PointerCenter")
            );
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (m_PointerHandler) {
            /*result = */m_PointerHandler->VOnPointerButtonUp(
                Point(msg.button.x,msg.button.y),
                1,
                msg.button.button == 1 ? "PointerLeft" :
                    (msg.button.button == 3 ? "PointerRight" :
                        "PointerCenter")
            );
        }
        break;
    default:
        break;
    }
    return false;
}

void HumanView::VOnUpdate(unsigned long deltaMs)
{
    m_pProcessManager->UpdateProcesses(deltaMs);
    for (auto i=m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i) {
        (*i)->VOnUpdate(deltaMs);
    }
}


bool HumanView::LoadGame(tinyxml2::XMLElement* pLevelData)
{
    // call the delegate method
    return VLoadGameDelegate(pLevelData);
}

void HumanView::VPushElement(std::shared_ptr<IScreenElement> pElement)
{
    m_ScreenElements.push_front(pElement);
}

void HumanView::VRemoveElement(std::shared_ptr<IScreenElement> pElement)
{
    m_ScreenElements.remove(pElement);
}

void HumanView::TogglePause(bool bActive)
{
    if (bActive) {
        //TODO
        /*if (g_pAudio) {
            g_pAudio->VPauseAllSounds();
        }*/
    } else {
        //TODO
        /*if (g_pAudio) {
            g_pAudio->VResumeAllSounds();
        }*/
    }
}

bool HumanView::InitAudio()
{
    //TODO
    //if (!g_pAudio) {
        //g_pAudio = new DirectSoundAudio();
    //}

    //TODO
    //if (!g_pAudio) {
    //    return false;
    //}

    //TODO
    //if (!g_pAudio->VInitialize(g_pApp->GetHwnd())) {
    //    return false;
    //}

    return true;
}

void HumanView::VSetCameraOffset(const Vec4& camOffset)
{
    assert(m_pCamera);
    if (m_pCamera) {
        m_pCamera->SetCameraOffset(camOffset);
    }
}

void HumanView::HandleGameState(BaseGameState newState)
{
    (void)newState;
    //TODO
}

void HumanView::PlaySoundDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataPlaySound> pCastEventData =
        std::static_pointer_cast<EvtDataPlaySound>(pEventData);
    (void)pCastEventData;

    //TODO
    //Resource resource(pCastEventData->GetResource().c_str());
    //std::shared_ptr<ResHandle> srh = std::static_pointer_cast<ResHandle>(
    //    g_pApp->m_pResCache->GetHandle(&resource)
    //);
    //std::shared_ptr<SoundProcess> sfx(new SoundProcess(srh, 100, false));
    //m_pProcessManager->AttachProcess(sfx);
}

void HumanView::GameStateDelegate(IEventDataPtr pEventData)
{
    // apparently not used in book...
}

void HumanView::RegisterAllDelegates()
{
    IEventManager* pGlobalEventManager = IEventManager::GetInstance();
    m_PlaySoundDelegate.reset(
        DECL_MBR_DELEGATE(&HumanView::PlaySoundDelegate)
    );
    pGlobalEventManager->VAddListener(
        m_PlaySoundDelegate,
        EvtDataPlaySound::sk_EventType
    );
}

void HumanView::RemoveAllDelegates()
{
    IEventManager* pGlobalEventManager = IEventManager::GetInstance();
    pGlobalEventManager->VRemoveListener(
        m_PlaySoundDelegate,
        EvtDataPlaySound::sk_EventType
    );
}


