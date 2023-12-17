#ifndef GCC4_EVENT_FACTORY_H_INCLUDED
#define GCC4_EVENT_FACTORY_H_INCLUDED

#include "GenericFactory.h"
#include "EventData.h"

extern GenericObjectFactory<IEventData,EventType> g_eventFactory;
#define REGISTER_EVENT(eventClass) g_eventFactory.Register<eventClass>(eventClass::sk_EventType)
#define CREATE_EVENT(eventType) g_eventFactory.Create(eventType)

#endif // GCC4_EVENT_FACTORY_H_INCLUDED

