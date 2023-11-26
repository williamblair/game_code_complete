#ifndef GCC4_DEFAULT_RESOURCE_LOADER_H_INCLUDED
#define GCC4_DEFAULT_RESOURCE_LOADER_H_INCLUDED

#include "IResourceLoader.h"

class DefaultResourceLoader : public IResourceLoader
{
public:

    virtual bool VUseRawFile() { return true; } // no additional processing needed
    virtual unsigned int VGetLoadedResourceSize( char* pRawBuffer,
                                                 unsigned int rawSize ) {
        return rawSize;
    }
    virtual bool VLoadResource( char* pRawBuffer,
                                unsigned int rawSize,
                                std::shared_ptr<ResHandle> handle ) {
        return true;
    }
    virtual std::string VGetPattern() { return ".*"; }
};

#endif // GCC4_DEFAULT_RESOURCE_LOADER_H_INCLUDED
