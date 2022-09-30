#ifndef IRESOURCE_LOADER_H_INCLUDED
#define IRESOURCE_LOADER_H_INCLUDED

#include <memory>
#include <string>

#include <ResHandle.h>

class IResourceLoader
{
public:
    virtual std::string VGetPattern() = 0; // file pattern to use with this loader
    virtual bool VUseRawFile() = 0;

    virtual unsigned int VGetLoadedResourceSize( char* pRawBuffer, 
                                                 unsigned int rawSize ) = 0;
    virtual bool VLoadResource( char* pRawBuffer, 
                                unsigned int rawSize, 
                                std::shared_ptr<ResHandle> handle ) = 0;
};

#endif // IRESOURCE_LOADER_H_INCLUDED
