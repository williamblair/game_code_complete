#ifndef GCC4_SCRIPT_RESOURCE_LOADER_H_INCLUDED
#define GCC4_SCRIPT_RESOURCE_LOADER_H_INCLUDED

#include "IResourceLoader.h"

class ScriptResourceLoader : public IResourceLoader
{
public:
    virtual std::string VGetPattern() {/*TODO*/return "*.lua";}
    virtual bool VUseRawFile() { /*TODO*/return false; }

    virtual unsigned int VGetLoadedResourceSize(
        char* pRawBuffer,
        unsigned int rawSize
    ) {
        //TODO
        return 0;
    }
    virtual bool VLoadResource(
        char* pRawBuffer,
        unsigned int rawSize,
        std::shared_ptr<ResHandle> handle
    ) {
        //TODO
        return true;
    }
};

std::shared_ptr<IResourceLoader> CreateScriptResourceLoader();

#endif // GCC4_SCRIPT_RESOURCE_LOADER_H_INCLUDED

