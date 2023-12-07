#include "TeapotEvents.h"

const EventType EvtDataFireWeapon::sk_EventType(0x1b15b6a7);
const EventType EvtDataStartThrust::sk_EventType(0x1d62d48c);
const EventType EvtDataEndThrust::sk_EventType(0xe60f88a4);
const EventType EvtDataStartSteer::sk_EventType(0xf0b5b4fd);
const EventType EvtDataEndSteer::sk_EventType(0x176645ef);

void RegisterTeapotScriptEvents()
{
    REGISTER_SCRIPT_EVENT(EvtDataFireWeapon,EvtDataFireWeapon::sk_EventType);
    REGISTER_SCRIPT_EVENT(EvtDataStartThrust,EvtDataStartThrust::sk_EventType);
    REGISTER_SCRIPT_EVENT(EvtDataEndThrust,EvtDataEndThrust::sk_EventType);
    REGISTER_SCRIPT_EVENT(EvtDataStartSteer,EvtDataStartSteer::sk_EventType);
    REGISTER_SCRIPT_EVENT(EvtDataEndSteer,EvtDataEndSteer::sk_EventType);
}

