#ifndef RES_CACHE_TYPES_H_INCLUDED
#define RES_CACHE_TYPES_H_INCLUDED

#include <memory>
#include <list>
#include <map>

// forward declarations
class ResHandle;
class IResourceLoader;

typedef std::list<std::shared_ptr<ResHandle>> ResHandleList;
typedef std::map<std::string, std::shared_ptr<ResHandle>> ResHandleMap;
typedef std::list<std::shared_ptr<IResourceLoader>> ResourceLoaders;

#endif // RES_CACHE_TYPES_H_INCLUDED
