#ifndef EVENT_DATA_H_INCLUDED
#define EVENT_DATA_H_INCLUDED

#include <iostream>
#include <sstream>
#include <memory>
#include <functional>

#include "ComponentTypes.h"

// forward declaration
class IEventData;

typedef unsigned long EventType;
//typedef unsigned long ActorId;
typedef std::shared_ptr<IEventData> IEventDataPtr;
typedef std::shared_ptr<std::function<void(IEventDataPtr)>> EventListenerDelegate;

// for declaring a delegate via a class member function
#define DECL_MBR_DELEGATE( func )               \
    new std::function<void(IEventDataPtr)>(     \
            std::bind( func,                    \
                       this,                    \
                       std::placeholders::_1    \
                )                               \
            ) 


class IEventData
{
public:
    virtual const EventType& VGetEventType() const = 0;
    virtual float VGetTimeStamp() const = 0;
    virtual void VSerialize( std::ostringstream& out ) const = 0;
    virtual IEventDataPtr VCopy() const = 0;
    virtual const char* GetName() const = 0;
};

class BaseEventData : public IEventData
{
public:
    explicit BaseEventData( const float timeStamp = 0.0f ) :
        m_timeStamp( timeStamp )
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

// example event data type
class EvtDataDestroyActor : public BaseEventData
{
public:
    static const EventType sk_EventType;

    explicit EvtDataDestroyActor( ActorId id ) :
        m_id( id )
    {}
    explicit EvtDataDestroyActor( std::istringstream& in )
    {
        in >> m_id;
    }
    
    virtual const EventType& VGetEventType() const { return sk_EventType; }

    virtual IEventDataPtr VCopy() const
    {
        return IEventDataPtr( new EvtDataDestroyActor(m_id) );
    }

    virtual void VSerialize( std::ostringstream& out ) const
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

#endif // EVENT_DATA_H_INCLUDED

