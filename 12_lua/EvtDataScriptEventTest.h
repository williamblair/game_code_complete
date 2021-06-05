#ifndef EVT_DATA_SCRIPT_EVENT_TEST
#define EVT_DATA_SCRIPT_EVENT_TEST

#include <ScriptEvent.h>

class EvtDataScriptEventFromLua : public ScriptEvent
{
public:

    static const EventType sk_EventType;

    EvtDataScriptEventFromLua();
    EvtDataScriptEventFromLua( int num ) :
        m_num( num )
    {}

    virtual const EventType& VGetEventType() const { return sk_EventType; }
    virtual const char* GetName() const { return "EvtDataScriptEventFromLua"; }
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr( new EvtDataScriptEventFromLua( m_num ) );
    }

    int GetVal() const { return m_num; }

    EXPORT_FOR_SCRIPT_EVENT( EvtDataScriptEventFromLua );

protected:

    virtual bool VBuildEventFromScript(); // extract value from Lua
    int m_num; // dummy test value
};

class EvtDataScriptEventToLua : public ScriptEvent
{
public:

    static const EventType sk_EventType;

    EvtDataScriptEventToLua();
    EvtDataScriptEventToLua( int num ) :
        m_num( num )
    {}

    virtual const EventType& VGetEventType() const { return sk_EventType; }
    virtual const char* GetName() const { return "EvtDataScriptEventToLua"; }
    virtual IEventDataPtr VCopy() const {
        return IEventDataPtr( new EvtDataScriptEventToLua( m_num ) );
    }

    EXPORT_FOR_SCRIPT_EVENT( EvtDataScriptEventToLua );

protected:

    virtual void VBuildEventData(); // fill data with value before sending to Lua
    int m_num;
};

#endif // EVT_DATA_SCRIPT_EVENT_TEST

