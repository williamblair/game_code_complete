#ifndef TEAPOT_EVENTS_H_INCLUDED
#define TEAPOT_EVENTS_H_INCLUDED

#include <GCC4/EventData.h>

class EvtDataFireWeapon : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataFireWeapon() :
        m_id(INVALID_ACTOR_ID)
    {}
    EvtDataFireWeapon(ActorId id) :
        m_id(id)
    {}

    virtual const EventType& VGetEventType() const {
        return sk_EventType;
    }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataFireWeapon(m_id));
    }

    virtual void VSerialize(std::ostringstream& out) const {
        out << m_id;
    }

    virtual const char* GetName() const { return "EvtDataFireWeapon"; }
    ActorId GetActorId() const { return m_id; }

    virtual bool VBuildEventFromScript()
    {
        //TODO
        return false;
    }

    EXPORT_FOR_SCRIPT_EVENT(EvtDataFireWeapon);

private:
    ActorId m_id;
};

class EvtDataStartThrust : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataStartThrust() :
        m_id(INVALID_ACTOR_ID),
        m_acceleration(0.0f)
    {}
    EvtDataStartThrust(const ActorId id, const float accel) :
        m_id(id),
        m_acceleration(accel)
    {}

    virtual const EventType& VGetEventType() const { return sk_EventType; }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataStartThrust(m_id,m_acceleration));
    }
    virtual void VSerialize(std::ostringstream& out) const {
        out << m_id << " " << m_acceleration << " ";
    }
    
    virtual const char* GetName() const { return "EvtDataStartThrust"; }
    ActorId GetActorId() const { return m_id; }
    float GetAcceleration() const { return m_acceleration; }

    void Set(const ActorId id) { m_id = id; }

    virtual bool VBuildEventFromScript()
    {
        //TODO
        return false;
    }

    EXPORT_FOR_SCRIPT_EVENT(EvtDataStartThrust);

private:
    ActorId m_id;
    float m_acceleration;
};

class EvtDataEndThrust : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataEndThrust() :
        m_id(INVALID_ACTOR_ID)
    {}
    EvtDataEndThrust(const ActorId id) :
        m_id(id)
    {}

    virtual const EventType& VGetEventType() const { return sk_EventType; }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataEndThrust(m_id));
    }
    virtual void VSerialize(std::ostringstream& out) const {
        out << m_id << " ";
    }
    
    virtual const char* GetName() const { return "EvtDataEndThrust"; }
    ActorId GetActorId() const { return m_id; }

    void Set(const ActorId id) { m_id = id; }

    virtual bool VBuildEventFromScript()
    {
        //TODO
        return false;
    }

    EXPORT_FOR_SCRIPT_EVENT(EvtDataEndThrust);

private:
    ActorId m_id;
};

class EvtDataStartSteer : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataStartSteer() :
        m_id(INVALID_ACTOR_ID),
        m_acceleration(0.0f)
    {}
    EvtDataStartSteer(const ActorId id, const float accel) :
        m_id(id),
        m_acceleration(accel)
    {}

    virtual const EventType& VGetEventType() const { return sk_EventType; }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataStartSteer(m_id,m_acceleration));
    }
    virtual void VSerialize(std::ostringstream& out) const {
        out << m_id << " " << m_acceleration << " ";
    }
    
    virtual const char* GetName() const { return "EvtDataStartSteer"; }
    ActorId GetActorId() const { return m_id; }
    float GetAcceleration() const { return m_acceleration; }

    void Set(const ActorId id) { m_id = id; }

    virtual bool VBuildEventFromScript()
    {
        //TODO
        return false;
    }

    EXPORT_FOR_SCRIPT_EVENT(EvtDataStartSteer);

private:
    ActorId m_id;
    float m_acceleration;
};

class EvtDataEndSteer : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataEndSteer() :
        m_id(INVALID_ACTOR_ID)
    {}
    EvtDataEndSteer(const ActorId id) :
        m_id(id)
    {}

    virtual const EventType& VGetEventType() const { return sk_EventType; }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataEndSteer(m_id));
    }
    virtual void VSerialize(std::ostringstream& out) const {
        out << m_id << " ";
    }
    
    virtual const char* GetName() const { return "EvtDataEndSteer"; }
    ActorId GetActorId() const { return m_id; }

    void Set(const ActorId id) { m_id = id; }

    virtual bool VBuildEventFromScript()
    {
        //TODO
        return false;
    }

    EXPORT_FOR_SCRIPT_EVENT(EvtDataEndSteer);

private:
    ActorId m_id;
};

class EvtDataGameplayUIUpdate : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataGameplayUIUpdate()
    {}
    EvtDataGameplayUIUpdate(const std::string& gameplayUiString) :
        m_gameplayUiString(gameplayUiString)
    {}

    virtual const EventType& VGetEventType() const {
        return sk_EventType;
    }

    //TODO
    //virtual void VDeserialize(std::istrstream& in) {
    //    in >> m_gameplayUiString;
    //}
    
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataGameplayUIUpdate(m_gameplayUiString));
    }

    virtual void VSerialize(std::ostringstream& out) const {
        out << m_gameplayUiString;
    }

    virtual const char* GetName() const { return "EvtDataGameplayUIUpdate"; }

    std::string GetGameplayUiString() const { return m_gameplayUiString; }

    virtual bool VBuildEventFromScript()
    {
        //TODO
        return false;
    }

    EXPORT_FOR_SCRIPT_EVENT(EvtDataGameplayUIUpdate);

private:
    std::string m_gameplayUiString;
};

class EvtDataSetControlledActor : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataSetControlledActor() { m_id = INVALID_ACTOR_ID; }
    EvtDataSetControlledActor(ActorId id) { m_id = id; }

    virtual const EventType& VGetEventType() const {
        return sk_EventType;
    }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataSetControlledActor(m_id));
    }

    virtual void VSerialize(std::ostringstream& out) const {
        out << m_id;
    }

    virtual const char* GetName() const { return "EvtDataSetControlledActor"; }
    ActorId GetActorId() const { return m_id; }

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

    EXPORT_FOR_SCRIPT_EVENT(EvtDataSetControlledActor);
private:
    ActorId m_id;
};

class EvtDataScriptEventTestToLua : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataScriptEventTestToLua() { m_num = 0; }
    EvtDataScriptEventTestToLua(int num) { m_num = num; }

    virtual const EventType& VGetEventType() const {
        return sk_EventType;
    }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataScriptEventTestToLua(m_num));
    }

    virtual void VSerialize(std::ostringstream& out) const {
        out << m_num;
    }

    virtual const char* GetName() const { return "EvtDataScriptEventTestToLua"; }
    int GetNum() const { return m_num; }

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

    EXPORT_FOR_SCRIPT_EVENT(EvtDataScriptEventTestToLua);
private:
    int m_num;
};

class EvtDataScriptEventTestFromLua : public ScriptEvent
{
public:
    static const EventType sk_EventType;

    EvtDataScriptEventTestFromLua() { m_num = 0; }
    EvtDataScriptEventTestFromLua(int num) { m_num = num; }

    virtual const EventType& VGetEventType() const {
        return sk_EventType;
    }

    //TODO
    /*virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }*/

    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr(new EvtDataScriptEventTestFromLua(m_num));
    }

    virtual void VSerialize(std::ostringstream& out) const {
        out << m_num;
    }

    virtual const char* GetName() const { return "EvtDataScriptEventTestFromLua"; }
    int GetNum() const { return m_num; }

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

    EXPORT_FOR_SCRIPT_EVENT(EvtDataScriptEventTestFromLua);
private:
    int m_num;
};

void RegisterTeapotScriptEvents();

#endif // TEAPOT_EVENTS_H_INCLUDED

