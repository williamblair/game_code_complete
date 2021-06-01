#ifndef SCRIPT_EVENT_H_INCLUDED
#define SCRIPT_EVENT_H_INCLUDED

#include <map>

#include <LuaPlus/LuaPlus.h>

#include <EventData.h>

// the # places quotes around the token
#define REGISTER_SCRIPT_EVENT( eventClass, eventType ) \
    ScriptEvent::RegisterEventTypeWithScript( #eventClass, eventType ); \
    ScriptEvent::AddCreationFunction( eventType, \
                                      &eventClass::CreateEventForScript )

#define EXPORT_FOR_SCRIPT_EVENT( eventClass ) \
    public: \
        static ScriptEvent* CreateEventForScript() \
        { \
            return new eventClass; \
        }

// forward declaration
class ScriptEvent;

typedef ScriptEvent* (*CreateEventForScriptFunctionType)(void);

class ScriptEvent : public BaseEventData
{
public:

    typedef std::map<EventType,CreateEventForScriptFunctionType> CreationFunctions;

    ScriptEvent();
    ~ScriptEvent();

    // should only be called from ScripExports functions
    LuaPlus::LuaObject GetEventData(); // called when event is sent from C++ to script
    bool SetEventData( LuaPlus::LuaObject eventData ); // called when event is sent from script to C++

    static void RegisterEventTypeWithScript( const char* key, EventType type );
    static void AddCreationFunction( EventType type, 
                                     CreateEventForScriptFunctionType pCreateFunc );
    static ScriptEvent* CreateEventFromScript( EventType type );


protected:

    static CreationFunctions s_creationFunctions; 
    bool m_eventDataIsValid;
    LuaPlus::LuaObject m_eventData; // data to/from Lua

    // to be implemented by subclasses
    virtual void VBuildEventData(); // fill m_eventData
    virtual bool VBuildEventFromScript() { return true; } // extract data from m_eventData
};

#endif // SCRIPT_EVENT_H_INCLUDED

