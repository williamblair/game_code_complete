#ifndef RESOURCE_ZIP_FILE_H_INCLUDED
#define RESOURCE_ZIP_FILE_H_INCLUDED

#include <ZipFile.h>
#include <IResourceFile.h>

class ResourceZipFile : public IResourceFile
{
public:

    ResourceZipFile( const std::string& resFileName ) {
        m_pZipFile = nullptr;
        m_resFileName = resFileName;
    }
    virtual ~ResourceZipFile();

    virtual bool VOpen();
    virtual int VGetRawResourceSize( const Resource& r );
    virtual int VGetRawResource( const Resource& r, char* buffer );
    virtual int VGetNumResources() const;
    virtual std::string VGetResourceName( int num ) const;

private:

    std::shared_ptr<ZipFile> m_pZipFile;
    std::string m_resFileName;
};

#endif // RESOURCE_ZIP_FILE_H_INCLUDED

