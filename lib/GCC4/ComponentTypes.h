#ifndef GCC4_COMPONENT_TYPES_H_INCLUDED
#define GCC4_COMPONENT_TYPES_H_INCLUDED

#include <cstdint>
#include <string>
#include <memory>
#include <map>

// forward declarations
class Actor;
class ActorComponent;

typedef uint64_t ActorId;
#define INVALID_ACTOR_ID 0xFFFFFFFFFFFFFFFF

typedef std::string ActorType;

typedef ActorComponent *(*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

typedef std::shared_ptr<Actor> StrongActorPtr;
typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;

typedef std::weak_ptr<Actor> WeakActorPtr;
typedef std::weak_ptr<ActorComponent> WeakActorComponentPtr;

typedef uint64_t ComponentId;



#endif // GCC4_COMPONENT_TYPES_H_INCLUDED

