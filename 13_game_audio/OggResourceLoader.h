#ifndef OGG_RESOURCE_LOADER_H_INCLUDED
#define OGG_RESOURCE_LOADER_H_INCLUDED

#include <GCC4/IResourceLoader.h>

class OggResourceLoader : public IResourceLoader
{
public:

    virtual std::string VGetPattern() { return "*.ogg"; }
    virtual bool VUseRawFile() { return false; }
    virtual unsigned int VGetLoadedResourceSize( char* pRawBuffer,
                                                 unsigned int rawSize );

    virtual bool VLoadResource( char* pRawBuffer,
                                unsigned int rawSize,
                                std::shared_ptr<ResHandle> handle );

protected:

    bool ParseOgg( char* oggStream,
                   size_t length,
                   std::shared_ptr<ResHandle> handle );
};

#endif // OGG_RESOURCE_LOADER_H_INCLUDED
