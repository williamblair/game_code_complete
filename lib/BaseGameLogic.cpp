#include <cassert>
#include <cstring>
#include "BaseGameLogic.h"
#include "GameCodeApp.h"
#include "Actor.h"
#include "Resource.h"
#include "MakeStrongPtr.h"
#include "ActorFactory.h"

bool LevelManager::Initialize(std::vector<std::string>& levels)
{
    std::vector<std::string>::iterator i = levels.begin();
    while (i != levels.end())
    {
        m_Levels.push_back(*i);
        ++i;
    }
    return true;
}

BaseGameLogic::BaseGameLogic() :
    m_State(BGS_Initializing)
{
    m_LastActorId = 0;
    m_LifeTime = 0;
    m_pProcessManager = new ProcessManager();
    m_Random.Randomize();
    m_State = BGS_Initializing;
    m_bProxy = false;
    m_bRenderDiagnostics = false;
    m_ExpectedPlayers = 0;
    m_ExpectedRemotePlayers = 0;
    m_ExpectedAi = 0;
    m_HumanPlayersAttached = 0;
    m_AiPlayersAttached = 0;
    m_HumanGamesLoaded = 0;
    //m_pPathingGraph = nullptr; /* TODO */
    m_pActorFactory = nullptr;

    m_pLevelManager = new LevelManager();
    assert(m_pLevelManager && m_pProcessManager);
    m_pLevelManager->Initialize(g_pApp->m_pResCache->Match("world\\*.xml"));

    //TODO
    //RegisterEngineScriptEvents();
}

BaseGameLogic::~BaseGameLogic()
{
    //TODO
    //while (!m_GameViews.empty()) {
    //    m_GameViews.pop_front();
    //}

    if (m_pLevelManager) { delete m_pLevelManager; }
    if (m_pProcessManager) { delete m_pProcessManager; }
    if (m_pActorFactory) { delete m_pActorFactory; }

    for (auto it=m_Actors.begin(); it != m_Actors.end(); ++it) {
        it->second->Destroy();
    }
    m_Actors.clear();

    IEventManager::GetInstance()->VRemoveListener(
        m_RequestNewActorDelegate,
        EvtDataDestroyActor::sk_EventType
    );
}

bool BaseGameLogic::Init()
{
    m_pActorFactory = VCreateActorFactory();
    //TODO
    //m_pPathingGraph.reset(CreatePathingGraph());

    m_RequestNewActorDelegate.reset(
        DECL_MBR_DELEGATE(&BaseGameLogic::RequestNewActorDelegate)
    );
    IEventManager::GetInstance()->VAddListener(
        m_RequestNewActorDelegate,
        EvtDataRequestDestroyActor::sk_EventType
    );

    return true;
}

std::string BaseGameLogic::GetActorXml(const ActorId id)
{
    StrongActorPtr pActor = MakeStrongPtr(VGetActor(id));
    if (pActor) {
        return pActor->ToXml();
    } else {
        char msg[256];
        sprintf(msg, "Couldn't find actor: %llu\n", (uint64_t)id);
        throw std::runtime_error(msg);
    }

    return "";
}

bool BaseGameLogic::VLoadGame(const char* levelResource)
{
    // Grab the root XML node
    XMLElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(levelResource);
    if (!pRoot) {
        throw std::runtime_error(
            "Failed to find level resource file: " +
            std::string(levelResource)
        );
    }

    // pre and post load scripts
    const char* preLoadScript = nullptr;
    const char* postLoadScript = nullptr;

    // parse the pre & post script attributes
    XMLElement* pScriptElem = pRoot->FirstChildElement("Script");
    if (pScriptElem) {
        preLoadScript = pScriptElem->Attribute("preLoad");
        postLoadScript = pScriptElem->Attribute("postLoad");
    }

    // Load the pre-load script if there is one
    if (preLoadScript) {
        Resource resource(preLoadScript);
        // This loads the XML file from the zip file
        std::shared_ptr<ResHandle> pResHandle =
            g_pApp->m_pResCache->GetHandle(&resource);
    }

    // load all initial actors
    XMLElement* pActorsNode = pRoot->FirstChildElement("StaticActors");
    if (pActorsNode)
    {
        for (XMLElement* pNode = pActorsNode->FirstChildElement();
            pNode;
            pNode = pNode->NextSiblingElement())
        {
            const char* actorResource = pNode->Attribute("resource");

            StrongActorPtr pActor = VCreateActor(actorResource, pNode);
            if (pActor) {
                // fire an event letting everyone else know we created an actor
                std::shared_ptr<EvtDataNewActor> pNewActorEvent(
                    new EvtDataNewActor(pActor->GetId())
                );
                IEventManager::GetInstance()->VQueueEvent(pNewActorEvent);
            }
        }
    }

    // Initialize all human views
    // TODO
    /*for (auto it = m_GameViews.begin(); it != m_GameViews.end(); ++it)
    {
        std::shared_ptr<IGameView> pView = *it;
        if (pView->VGetType() == GameView_Human) {
            std::shared_ptr<HumanView> pHumanView =
                std::static_pointer_cast<HumanView,IGameView>(pView);
            pHumanView->LoadGame(pRoot);
        }
    }*/

    // call the delegate load function
    if (!VLoadGameDelegate(pRoot)) {
        // Assumes VLoadGameDelegate printed error message
        return false;
    }

    // Load the post-load script if there is one
    if (postLoadScript) {
        Resource resource(postLoadScript);
        // this loads the XML file from the zip file
        std::shared_ptr<ResHandle> pResHandle =
            g_pApp->m_pResCache->GetHandle(&resource);
    }

    // trigger the environment loaded game event
    // only then can player actors and AI be spawned
    if (m_bProxy) {
        std::shared_ptr<EvtDataRemoteEnvironmentLoaded> pEvent(
            new EvtDataRemoteEnvironmentLoaded()
        );
        IEventManager::GetInstance()->VTriggerEvent(pEvent);
    } else {
        std::shared_ptr<EvtDataEnvironmentLoaded> pEvent(
            new EvtDataEnvironmentLoaded()
        );
        IEventManager::GetInstance()->VTriggerEvent(pEvent);
    }

    return true;
}

void BaseGameLogic::VSetProxy()
{
    m_bProxy = true;
    m_RequestNewActorDelegate.reset(
        DECL_MBR_DELEGATE(&BaseGameLogic::RequestNewActorDelegate)
    );
    IEventManager::GetInstance()->VAddListener(
        m_RequestNewActorDelegate,
        EvtDataRequestNewActor::sk_EventType
    );

    //TODO
    //m_pPhysics.reset(CreateNullPhysics());
}

StrongActorPtr BaseGameLogic::VCreateActor(
    const std::string& actorResource,
    XMLElement* overrides,
    const Mat4x4* initialTransform,
    const ActorId serversActorId)
{
    assert(m_pActorFactory);
    if (!m_bProxy && serversActorId != INVALID_ACTOR_ID) {
        return StrongActorPtr();
    }
    if (m_bProxy && serversActorId == INVALID_ACTOR_ID) {
        return StrongActorPtr();
    }

    StrongActorPtr pActor = m_pActorFactory->CreateActor(
        actorResource.c_str(),
        overrides,
        initialTransform,
        serversActorId
    );
    if (pActor) {
        m_Actors.insert(std::make_pair(pActor->GetId(),pActor));
        if (!m_bProxy &&
            (m_State==BGS_SpawnPlayersActors || m_State==BGS_Running))
        {
            std::shared_ptr<EvtDataRequestNewActor> pNewActor(
                new EvtDataRequestNewActor(
                    actorResource,
                    initialTransform,
                    pActor->GetId()
                )
            );
            IEventManager::GetInstance()->VTriggerEvent(pNewActor);
        }
        return pActor;
    }
    // TODO - error out
    return StrongActorPtr();
}

void BaseGameLogic::VDestroyActor(const ActorId actorId)
{
    // we need to trigger a synchronous event to ensure that
    // any systems responding to this event can still access a
    // valid actor if need be. The actor will be destroyed after this.
    std::shared_ptr<EvtDataDestroyActor> pEvent(new EvtDataDestroyActor(actorId));
    IEventManager::GetInstance()->VTriggerEvent(pEvent);

    auto findIt = m_Actors.find(actorId);
    if (findIt != m_Actors.end()) {
        findIt->second->Destroy();
        m_Actors.erase(findIt);
    }
}

WeakActorPtr BaseGameLogic::VGetActor(const ActorId actorId)
{
    ActorMap::iterator findIt = m_Actors.find(actorId);
    if (findIt != m_Actors.end()) {
        return findIt->second;
    }
    return WeakActorPtr();
}

void BaseGameLogic::VModifyActor(const ActorId actorId, XMLElement* overrides)
{
    assert(m_pActorFactory);

    auto findIt = m_Actors.find(actorId);
    if (findIt != m_Actors.end()) {
        //TODO
        //m_pActorFactor->ModifyActor(findIt->second, overrides);
    }
}

void BaseGameLogic::VOnUpdate(float time, float elapsedTime)
{
    int deltaMilliseconds = int(elapsedTime*1000.0f);
    m_LifeTime += elapsedTime;

    switch (m_State)
    {
    case BGS_Initializing:
        // If we get to here we're ready to attach players
        VChangeState(BGS_MainMenu);
        break;

    case BGS_MainMenu:
        break;
    
    case BGS_LoadingGameEnvironment:
        break;
    
    case BGS_WaitingForPlayersToLoadEnvironment:
        if (m_ExpectedPlayers + m_ExpectedRemotePlayers <= m_HumanGamesLoaded)
        {
            VChangeState(BGS_SpawnPlayersActors);
        }
        break;
    
    case BGS_SpawnPlayersActors:
        VChangeState(BGS_Running);
        break;
    
    case BGS_WaitingForPlayers:
        if (m_ExpectedPlayers + m_ExpectedRemotePlayers == m_HumanPlayersAttached)
        {
            // The server sends us the level name as a part of the login message.
            // we have to wait until it arrives before loading the level
            //TODO
            /*if (!g_pApp->m_Options.m_Level.empty())
            {
                VChangeState(BGS_LoadingGameEnvironment);
            }*/
        }
        break;

    case BGS_Running:
        m_pProcessManager->UpdateProcesses(deltaMilliseconds);

        // TODO
        /*if (m_pPhysics && !m_bProxy)
        {
            m_pPhysics->VOnUpdate(elapsedTime);
            m_pPhysics->VSyncVisibleScene();
        }*/

        break;
    
    default:
        throw std::runtime_error("Unrecognized State.");
    }

    // update all game views
    //TODO
    /*for (GameViewList::iterator it = m_GameViews.begin(); it != m_GameViews.end(); ++it)
    {
        (*it)->VOnUpdate(deltaMilliseconds);
    }*/

    // update game actors
    for (ActorMap::const_iterator it = m_Actors.begin(); it != m_Actors.end(); ++it)
    {
        it->second->Update(deltaMilliseconds);
    }
}

void BaseGameLogic::VChangeState(BaseGameState newState)
{
    if (newState == BGS_WaitingForPlayers)
    {
        // Get rid of the main menu
        //TODO
        // m_GameViews.pop_front();

        // Note - split screen support would require this to change!
        m_ExpectedPlayers = 1;
        //TODO
        //m_ExpectedRemotePlayers = g_pApp->m_Options.m_expectedPlayers - 1;
        //m_ExpectedAi = g_pApp->m_Options.m_NumAis;

        if (!g_pApp->m_Options.m_gameHost.empty())
        {
            VSetProxy();
            m_ExpectedAi = 0; // the server will create these
            m_ExpectedRemotePlayers = 0;

            if (!g_pApp->AttachAsClient()) {
                // throw up a main menu
                VChangeState(BGS_MainMenu);
                return;
            }
        } else if (m_ExpectedRemotePlayers > 0) {
            //TODO
            /*BaseSockerManager* pServer = new BaseSocketManger();
            if (!pServer->Init()) {
                // throw up a main menu
                VChangeState(BGS_MainMenu);
            }

            pServer->AddSocket(new GameServerListenSocket(g_pApp->m_Options.m_listenPort));
            g_pApp->m_pBaseSocketManager = pServer;*/
        }
    } // end if newState == BGS_WaitingForPlayers
    else if (newState == BGS_LoadingGameEnvironment)
    {
        m_State = newState;
        if (!g_pApp->VLoadGame()) {
            throw std::runtime_error("The game failed to load.");
            g_pApp->AbortGame();
        } else {
            // we must wait for all human players to report that
            // their environments are loaded
            VChangeState(BGS_WaitingForPlayersToLoadEnvironment);
            return;
        }
    }

    m_State = newState;
}

void BaseGameLogic::VRenderDiagnostics()
{
    if (m_bRenderDiagnostics) {
        //TODO
        //m_pPhysics->VRenderDiagnostics();
    }
}

//TODO
/*
void BaseGameLogic::VAddView(std::shared_ptr<IGameView> pView, ActorId actorId)
{
    // This makes sure that all views have a non-zero view id
    int viewId = static_cast<int>(m_GameViews.size());
    m_GameViews.push_back(pView);
    pView->VOnAttach(viewId, actorId);
    pView->VOnRestore();
}*/

//TODO
/*
void BaseGameLogic::VRemoveView(std::shared_ptr<IGameView> pView)
{
    m_GameViews.remove(pView);
}*/

ActorFactory* BaseGameLogic::VCreateActorFactory()
{
    return new ActorFactory();
}

void BaseGameLogic::RequestDestroyActorDelegate(IEventDataPtr pEventData)
{
    std::shared_ptr<EvtDataRequestDestroyActor> pCastEventData =
        std::static_pointer_cast<EvtDataRequestDestroyActor>(pEventData);
    VDestroyActor(pCastEventData->GetActorId());
}

void BaseGameLogic::MoveActorDelegate(IEventDataPtr pEventData)
{
    //std::shared_ptr<EvtData_Move_Actor> pCastEventData =
    //    std::static_pointer_cast<EvtData_Move_Actor>(pEventData);
    std::shared_ptr<EvtDataMoveActor> pCastEventData =
        std::static_pointer_cast<EvtDataMoveActor>(pEventData);
    VMoveActor(pCastEventData->GetId(), pCastEventData->GetMatrix());
}

void BaseGameLogic::RequestNewActorDelegate(IEventDataPtr pEventData)
{
    // This should only happen if the game logic is a proxy,
    // and there's a server asking us to create an actor.
    if (!m_bProxy) {
        return;
    }

    //std::shared_ptr<EvtData_Request_New_Actor> pCastEventData =
    //    std::static_pointer_cast<EvtData_Request_New_Actor>(pEventData);
    std::shared_ptr<EvtDataRequestNewActor> pCastEventData =
        std::static_pointer_cast<EvtDataRequestNewActor>(pEventData);
    
    StrongActorPtr pActor = VCreateActor(
        pCastEventData->GetActorResource(),
        nullptr,
        pCastEventData->GetInitialTransform(),
        pCastEventData->GetServerActorId()
    );
    if (pActor) {
        std::shared_ptr<EvtDataNewActor> pEvent(
            new EvtDataNewActor(pActor->GetId()/*, pCastEventData->GetViewId()*/)
        );
        IEventManager::GetInstance()->VQueueEvent(pEvent);
    }
}

