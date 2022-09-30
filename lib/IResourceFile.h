#ifndef IRESOURCE_FILE_H_INCLUDED
#define IRESOURCE_FILE_H_INCLUDED

#include <Resource.h>

class IResourceFile
{
public:
    virtual ~IResourceFile() {}
    
    virtual bool VOpen() = 0;
    virtual int VGetRawResourceSize( const Resource& r ) = 0; // how large the buffer should be to get resource
    virtual int VGetRawResource( const Resource& r, char* buffer ) = 0;
    virtual int VGetNumResources() const = 0;
    virtual std::string VGetResourceName( int num ) const = 0;
};

#endif // IRESOURCE_FILE_H_INCLUDED

