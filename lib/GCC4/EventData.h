#ifndef GCC4_EVENT_DATA_H_INCLUDED
#define GCC4_EVENT_DATA_H_INCLUDED

#include <iostream>
#include <sstream>
#include <memory>
#include <functional>

#include <LuaPlus/LuaPlus.h>

#include "ComponentTypes.h"
#include "GCCMath.h"
#include "IPacket.h" // for socket defs
#include "IGameView.h"

// forward declaration
class IEventData;

typedef unsigned long EventType;
//typedef unsigned long ActorId;
typedef std::shared_ptr<IEventData> IEventDataPtr;
typedef std::shared_ptr<std::function<void(IEventDataPtr)>> EventListenerDelegate;

// Lua scripting stuff
class ScriptEvent;
typedef ScriptEvent* (*CreateEventForScriptFunctionType)(void);
#define REGISTER_SCRIPT_EVENT(eventClass, eventType) \
    ScriptEvent::RegisterEventTypeWithScript(#eventClass, eventType); \
    ScriptEvent::AddCreationFunction(eventType,&eventClass::CreateEventForScript)
#define EXPORT_FOR_SCRIPT_EVENT(eventClass) \
    public: \
        static ScriptEvent* CreateEventForScript() \
        { \
            return new eventClass; \
        }

void RegisterEngineScriptEvents();

// for declaring a delegate via a class member function
#define DECL_MBR_DELEGATE( func )               \
    new std::function<void(IEventDataPtr)>(     \
            std::bind( func,                    \
                       this,                    \
                       std::placeholders::_1    \
                )                               \
            ) 

// for declaring a delegate via a class member function
// with a class instance pointer
#define DECL_MBR_DELEGATE_THIS( func, pThis ) \
    new std::function<void(IEventDataPtr)>(     \
            std::bind( func,                    \
                       pThis,                   \
                       std::placeholders::_1    \
                )                               \
            ) 


class IEventData
{
public:
    virtual const EventType& VGetEventType() const = 0;
    virtual float VGetTimeStamp() const = 0;
    virtual void VSerialize(std::ostringstream& out) const = 0;
    virtual IEventDataPtr VCopy() const = 0;
    virtual const char* GetName() const = 0;
};

class BaseEventData : public IEventData
{
public:
    explicit BaseEventData(const float timeStamp = 0.0f) :
        m_timeStamp(timeStamp)
    {}
    virtual ~BaseEventData()
    {}

    virtual const EventType& VGetEventType() const = 0;
    float VGetTimeStamp() const { return m_timeStamp; }

    // for example purposes uses ostringstream so it can be human
    // readable, but std::ostream would be better because it's
    // faster (binary)
    virtual void VSerialize( std::ostringstream& out ) const {}

protected:
    const float m_timeStamp;
};

class EvtDataNewActor : public BaseEventData
{
public:

    static const EventType sk_EventType;

    EvtDataNewActor() {
        m_ActorId = INVALID_ACTOR_ID;
        m_ViewId = gc_InvalidGameViewId;
    }

    explicit EvtDataNewActor(
        ActorId actorId,
        GameViewId viewId = gc_InvalidGameViewId)
    {
        m_ActorId = actorId;
        m_ViewId = viewId;
    }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in)
    {
        in >> m_ActorId;
        in >> m_ViewId;
    }*/

    virtual const EventType& VGetEventType() const {
        return sk_EventType;
    }

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataNewActor(m_ActorId, m_ViewId)
        );
    }

    virtual void VSerialize(std::ostringstream& out) const {
        out << m_ActorId << " "
            << m_ViewId << " ";
    }

    virtual const char* GetName() const {
        return "EvtDataNewActor";
    }

    const ActorId GetActorId() const { return m_ActorId; }
    const GameViewId GetViewId() const { return m_ViewId; }

private:
    ActorId m_ActorId;
    GameViewId m_ViewId;
};

// This event is sent when a client has loaded its environment.
// This is special because we only want this event to go from client to server,
// and stop there. the EvtDataEnvironmentLoaded is received by server and proxy logics alike.
// thy to do this with just the above event and you'll get into an endless loop of the
// EvtDataEnvironmentLoaded event making infinite round trips from client to server.
class EvtDataRemoteEnvironmentLoaded : public BaseEventData
{
public:
    static const EventType sk_EventType;
    EvtDataRemoteEnvironmentLoaded() {}
    virtual const EventType& VGetEventType() const { return sk_EventType; }
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataRemoteEnvironmentLoaded()
        );
    }
    virtual const char* GetName() const { return "EvtDataRemoteEnvironmentLoaded"; }
};

// This is sent when a new game is started
class EvtDataEnvironmentLoaded : public BaseEventData
{
public:
    static const EventType sk_EventType;
    EvtDataEnvironmentLoaded() {}
    virtual const EventType& VGetEventType() const { return sk_EventType; }
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataEnvironmentLoaded()
        );
    }
    virtual const char* GetName() const { return "EvtDataEnvironmentLoaded"; }
};

// example event data type
class EvtDataDestroyActor : public BaseEventData
{
public:
    static const EventType sk_EventType;

    explicit EvtDataDestroyActor() :
        m_id(INVALID_ACTOR_ID)
    {}
    explicit EvtDataDestroyActor(ActorId id) :
        m_id(id)
    {}
    explicit EvtDataDestroyActor(std::istringstream& in)
    {
        in >> m_id;
    }
    
    virtual const EventType& VGetEventType() const { return sk_EventType; }

    virtual IEventDataPtr VCopy() const
    {
        return IEventDataPtr(new EvtDataDestroyActor(m_id));
    }

    virtual void VSerialize(std::ostringstream& out) const
    {
        out << m_id;
    }

    virtual const char* GetName() const 
    {
        return "EvtDataDestroyActor";
    }

    // specific to this event data type
    ActorId GetId() const { return m_id; }

private:
    ActorId m_id;
};

class ScriptEvent : public BaseEventData
{
public:
    ScriptEvent() { m_eventDataIsValid = false; }

    LuaPlus::LuaObject GetEventData(); // called when event sent from c++ to script
    bool SetEventData(LuaPlus::LuaObject eventData); // called when event is sent from script to c++

    // helper functions for registering with the script. You should call REGISTER_SCRIPT_EVENT()
    // instead of calling this directly. Any class that needs to be exported also needs to
    // call the EXPORT_FOR_SCRIPT_EVENT() inside the class declaration.
    static void RegisterEventTypeWithScript(const char* key, EventType type);
    static void AddCreationFunction(
        EventType type,
        CreateEventForScriptFunctionType pCreationFunctionPtr
    );
    static ScriptEvent* CreateEventFromScript(EventType type);

protected:
    typedef std::map<EventType,CreateEventForScriptFunctionType> CreationFunctions;
    static CreationFunctions s_creationFunctions;

    bool m_eventDataIsValid;
    LuaPlus::LuaObject m_eventData;

    // This function must be overridden if you want to fire this event in c++ and have it
    // received by the script. If you only fire the event from the script side, this function
    // will never be called. Its purpose is to fill in any c++ member vars using the data in
    // m_EventData which is valid at the time of the call. It is called when the vent is fired
    // from the script. return false if the data is invalid in some way, which will keep the event
    // from actually firing.
    virtual void VBuildEventData();

    // This function must be overridden if you want to fire this event from the Script and have
    // it received by c++. if you only fire this event from the script and received by the script,
    // it doesn't matter since m_EventData will just be passed straight through. Its purpose is to
    // fill in any c++ member variables using the data in m_EventData which is valid at the time
    // of the call. it is called when the event is fired from the script. return false if the data
    // is invalid in some way, which will keep the event from actually firing.
    virtual bool VBuildEventFromScript() { return true; }
};

class EvtDataRequestDestroyActor : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataRequestDestroyActor() { m_ActorId = INVALID_ACTOR_ID; }
    EvtDataRequestDestroyActor(ActorId id) { m_ActorId = id; }

    virtual const EventType& VGetEventType() const {
        return sk_EventType;
    }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_ActorId;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataRequestDestroyActor(m_ActorId));
    }

    virtual void VSerialize(std::ostringstream& out) const {
        out << m_ActorId;
    }

    virtual const char* GetName() const { return "EvtDataRequestDestroyActor"; }
    ActorId GetActorId() const { return m_ActorId; }

    virtual bool VBuildEventFromScript()
    {
        //TODO
        /*if (m_EventData.IsInteger()) {
            m_ActorId = m_EventData.GetInteger();
            return true;
        }
        return false;*/
        return false;
    }

    EXPORT_FOR_SCRIPT_EVENT(EvtDataRequestDestroyActor);

private:
    ActorId m_ActorId;
};

class EvtDataRequestNewActor : public BaseEventData
{
public:
    static const EventType sk_EventType;
    EvtDataRequestNewActor()
    {
        m_ActorResource = "";
        m_bHasInitialTransform = false;
        m_InitialTransform = Mat4x4::g_Identity;
        m_ServerActorId = -1;
        m_ViewId = gc_InvalidGameViewId;
    }
    explicit EvtDataRequestNewActor(
        const std::string& actorResource,
        const Mat4x4* initialTransform = nullptr,
        const ActorId serverActorId = INVALID_ACTOR_ID,
        const GameViewId viewId = gc_InvalidGameViewId)
    {
        m_ActorResource = actorResource;
        if (initialTransform) {
            m_bHasInitialTransform = true;
            m_InitialTransform = *initialTransform;
        } else {
            m_bHasInitialTransform = false;
        }
        m_ServerActorId = serverActorId;
        m_ViewId = viewId;
    }
    virtual const EventType& VGetEventType() const { return sk_EventType; }
    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_ActorResource;
        in >> m_bHasInitialTransform;
        if (m_bHasInitialTransform) {
            for (int i=0; i<4; ++i) {
                for (int j=0; j<4; ++j) {
                    in >> m_InitialTransform.v[i][j];
                }
            }
        }
        in >> m_ServerActorId;
        //TODO
        //in >> m_ViewId;
    }*/
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataRequestNewActor(
                m_ActorResource,
                m_bHasInitialTransform ? &m_InitialTransform : nullptr,
                m_ServerActorId,
                m_ViewId
            )
        );
    }
    virtual void VSerialize(std::ostringstream& out) const {
        out << m_ActorResource << " ";
        out << m_bHasInitialTransform << " ";
        if (m_bHasInitialTransform) {
            for (int i=0; i<4; ++i) {
                for (int j=0; j<4; ++j) {
                    out << m_InitialTransform.v[i][j] << " ";
                }
            }
        }
        out << m_ServerActorId << " ";
        out << m_ViewId << " ";
    }
    virtual const char* GetName() const { return "EvtDataRequestNewActor"; }

    const std::string& GetActorResource() const { return m_ActorResource; }
    const Mat4x4* GetInitialTransform() const { return m_bHasInitialTransform ? &m_InitialTransform : nullptr; }
    const ActorId GetServerActorId() const { return m_ServerActorId; }
    GameViewId GetViewId() const { return m_ViewId; }
private:
    std::string m_ActorResource;
    bool m_bHasInitialTransform;
    Mat4x4 m_InitialTransform;
    ActorId m_ServerActorId;
    GameViewId m_ViewId;
};

// Sent when actors are moved
class EvtDataMoveActor : public BaseEventData
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }
    EvtDataMoveActor() {
        m_Id = INVALID_ACTOR_ID;
    }
    EvtDataMoveActor(ActorId id, const Mat4x4& matrix) {
        m_Id = id;
        m_Matrix = matrix;
    }
    virtual void VSerialize(std::ostringstream& out) const {
        out << m_Id << " ";
        for (int i=0; i<4; ++i) {
            for (int j=0; j<4; ++j) {
                out << m_Matrix.v[i][j] << " ";
            }
        }
    }
    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_Id;
        for (int i=0; i<4; ++i) {
            for (int j=0; j<4; ++j) {
                in >> m_Matrix.v[i][j];
            }
        }
    }*/
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataMoveActor(m_Id, m_Matrix)
        );
    }
    virtual const char* GetName() const { return "EvtDataMoveActor"; }
    
    ActorId GetId() const { return m_Id; }
    const Mat4x4& GetMatrix() const { return m_Matrix; }

private:
    ActorId m_Id;
    Mat4x4 m_Matrix;
};

class EvtDataPhysCollision : public ScriptEvent
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }

    EvtDataPhysCollision() :
        m_ActorA(INVALID_ACTOR_ID),
        m_ActorB(INVALID_ACTOR_ID),
        m_SumNormalForce(0.0f,0.0f,0.0f),
        m_SumFrictionForce(0.0f,0.0f,0.0f)
    {
    }

    explicit EvtDataPhysCollision(
            ActorId actorA,
            ActorId actorB,
            Vec3 sumNormalForce,
            Vec3 sumFrictionForce,
            Vec3List collisionPoints) :
        m_ActorA(actorA),
        m_ActorB(actorB),
        m_SumNormalForce(sumNormalForce),
        m_SumFrictionForce(sumFrictionForce),
        m_CollisionPoints(collisionPoints)
    {}

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataPhysCollision(
                m_ActorA,
                m_ActorB,
                m_SumNormalForce,
                m_SumFrictionForce,
                m_CollisionPoints
            )
        );
    }

    virtual const char* GetName() const { return "EvtDataPhysCollision"; }

    ActorId GetActorA() const { return m_ActorA; }
    ActorId GetActorB() const { return m_ActorB; }
    const Vec3& GetSumNormalForce() const { return m_SumNormalForce; }
    const Vec3& GetSumFrictionForce() const { return m_SumFrictionForce; }
    const Vec3List& GetCollisionPoints() const { return m_CollisionPoints; }

    virtual void VBuildEventData();

    EXPORT_FOR_SCRIPT_EVENT(EvtDataPhysCollision);

private:
    ActorId m_ActorA;
    ActorId m_ActorB;
    Vec3 m_SumNormalForce;
    Vec3 m_SumFrictionForce;
    Vec3List m_CollisionPoints;
};

class EvtDataPlaySound : public ScriptEvent
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }

    EvtDataPlaySound()
    {}
    EvtDataPlaySound(const std::string& soundResource) :
        m_soundResource(soundResource)
    {}

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataPlaySound(
                m_soundResource
            )
        );
    }

    virtual const char* GetName() const { return "EvtDataPlaySound"; }

    virtual void VBuildEventData();

    EXPORT_FOR_SCRIPT_EVENT(EvtDataPlaySound);

    const std::string& GetResource() const { return m_soundResource; }

private:
    std::string m_soundResource;
};

class EvtDataPhysTriggerEnter : public BaseEventData
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }
    
    EvtDataPhysTriggerEnter() :
        m_TriggerId(-1),
        m_Other(INVALID_ACTOR_ID)
    {}
    explicit EvtDataPhysTriggerEnter(int triggerId, ActorId other) :
        m_TriggerId(triggerId),
        m_Other(other)
    {}
    IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataPhysTriggerEnter(m_TriggerId, m_Other)
        );
    }
    virtual const char* GetName() const { return "EvtDataPhysTriggerEnter"; }

    int GetTriggerId() const { return m_TriggerId; }
    ActorId GetOtherActor() const { return m_Other; }

private:
    int m_TriggerId;
    ActorId m_Other;
};

class EvtDataPhysTriggerLeave : public BaseEventData
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }

    EvtDataPhysTriggerLeave() :
        m_TriggerId(-1),
        m_Other(INVALID_ACTOR_ID)
    {}
    explicit EvtDataPhysTriggerLeave(int triggerId, ActorId other) :
        m_TriggerId(triggerId),
        m_Other(other)
    {}

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataPhysTriggerLeave(m_TriggerId, m_Other)
        );
    }
    virtual const char* GetName() const { return "EvtDataPhysTriggerLeave"; }

    int GetTriggerId() const { return m_TriggerId; }
    ActorId GetOtherActor() const { return m_Other; }

private:
    int m_TriggerId;
    ActorId m_Other;
};

class EvtDataPhysSeparation : public BaseEventData
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }

    EvtDataPhysSeparation() :
        m_ActorA(INVALID_ACTOR_ID),
        m_ActorB(INVALID_ACTOR_ID)
    {}
    explicit EvtDataPhysSeparation(ActorId actorA, ActorId actorB) :
        m_ActorA(actorA),
        m_ActorB(actorB)
    {}
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataPhysSeparation(m_ActorA, m_ActorB)
        );
    }
    virtual const char* GetName() const { return "EvtDataPhysSeparation"; }

    ActorId GetActorA() const { return m_ActorA; }
    ActorId GetActorB() const { return m_ActorB; }

private:
    ActorId m_ActorA;
    ActorId m_ActorB;
};

// Networking stuff

// Sent whenever a new client attaches to a game logic acting as a server
class EvtDataRemoteClient : public BaseEventData
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }

    explicit EvtDataRemoteClient(
        int socketId,
        int ipAddress)
    {
        m_socketId = socketId;
        m_ipAddress = ipAddress;
    }
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataRemoteClient(
                m_socketId,
                m_ipAddress
            )
        );
    }

    virtual void VSerialize(std::ostringstream& out) const {
        out << m_socketId << " ";
        out << m_ipAddress;
    }
    // TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_socketId;
        in >> m_ipAddress;
    }*/
    virtual const char* GetName() const { return "EvtDataRemoteClient"; }

    int GetSocketId() const { return m_socketId; }
    int GetIpAddress() const { return m_ipAddress; }

private:
    int m_socketId;
    int m_ipAddress;
};

// sent by the server to the clients when a network view
// is assigned a player number
class EvtDataNetworkPlayerActorAssignment : public BaseEventData
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }
    
    EvtDataNetworkPlayerActorAssignment() :
        m_ActorId(INVALID_ACTOR_ID),
        m_SocketId(INVALID_SOCKET)
    {
    }
    explicit EvtDataNetworkPlayerActorAssignment(
        const ActorId actorId,
        const int socketId)
    {
        m_ActorId = actorId;
        m_SocketId = socketId;
    }
    
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataNetworkPlayerActorAssignment(m_ActorId, m_SocketId)
        );
    }
    
    virtual const char* GetName() const {
        return "EvtDataNetworkPlayerActorAssignment";
    }
    
    virtual void VSerialize(std::ostringstream& out) const {
        out << m_ActorId << " ";
        out << m_SocketId;
    }
    
    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_ActorId;
        in >> m_SocketId;
    }*/
    
    // specific to this event type
    ActorId GetActorId() const { return m_ActorId; }
    int GetSocketId() const { return m_SocketId; }
    
private:
    ActorId m_ActorId;
    int m_SocketId;
};


// For background-threaded unzip/resource load
class EvtDataDecompressRequest : public BaseEventData
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }
    EvtDataDecompressRequest(
            const std::string& zipFileName,
            const std::string& fileName) :
        m_zipFileName(zipFileName),
        m_fileName(fileName)
    {
    }
    virtual void VSerialize(std::ostringstream& out) const {
        /*
        out << m_Id << " ";
        for (int i=0; i<4; ++i) {
            for (int j=0; j<4; ++j) {
                out << m_Matrix.v[i][j] << " ";
            }
        }*/
        //TODO
    }
    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_Id;
        for (int i=0; i<4; ++i) {
            for (int j=0; j<4; ++j) {
                in >> m_Matrix.v[i][j];
            }
        }
    }*/
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataDecompressRequest(
                m_zipFileName,
                m_fileName
            )
        );
    }
    virtual const char* GetName() const { return "EvtDataDecompressRequest"; }
    
    std::string m_zipFileName;
    std::string m_fileName;
};

class EvtDataDecompressProgress : public BaseEventData
{
public:
    static const EventType sk_EventType;
    virtual const EventType& VGetEventType() const { return sk_EventType; }
    EvtDataDecompressProgress(
            int progressPercent,
            const std::string& zipFileName,
            const std::string fileName,
            char* buffer) :
        m_progressPercent(progressPercent),
        m_zipFileName(zipFileName),
        m_fileName(fileName),
        m_dataBuffer(buffer)
    {
    }
    virtual void VSerialize(std::ostringstream& out) const {
        /*
        out << m_Id << " ";
        for (int i=0; i<4; ++i) {
            for (int j=0; j<4; ++j) {
                out << m_Matrix.v[i][j] << " ";
            }
        }*/
        //TODO
    }
    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_Id;
        for (int i=0; i<4; ++i) {
            for (int j=0; j<4; ++j) {
                in >> m_Matrix.v[i][j];
            }
        }
    }*/
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataDecompressProgress(
                m_progressPercent,
                m_zipFileName,
                m_fileName,
                m_dataBuffer
            )
        );
    }
    virtual const char* GetName() const { return "EvtDataDecompressProgress"; }


    int m_progressPercent;
    std::string m_zipFileName;
    std::string m_fileName;
    char* m_dataBuffer;
};

class EvtDataRequestStartGame : public BaseEventData
{
public:

    static const EventType sk_EventType;

    EvtDataRequestStartGame() {}

    //TODO
    /*virtual void VDeserialize(std::istrstream& in)
    {
        in >> m_ActorId;
        in >> m_ViewId;
    }*/

    virtual const EventType& VGetEventType() const {
        return sk_EventType;
    }

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(
            new EvtDataRequestStartGame()
        );
    }

    virtual const char* GetName() const {
        return "EvtDataRequestStartGame";
    }
};

#endif // GCC4_EVENT_DATA_H_INCLUDED


