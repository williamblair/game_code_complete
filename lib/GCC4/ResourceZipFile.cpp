//#include <optional>
#include <memory>
#include "ResourceZipFile.h"

ResourceZipFile::~ResourceZipFile()
{
}

bool ResourceZipFile::VOpen()
{
    m_pZipFile = std::unique_ptr<ZipFile>(new ZipFile);
    if ( !m_pZipFile ) {
        return false;
    }

    return m_pZipFile->Init( m_resFileName.c_str() );
}

int ResourceZipFile::VGetRawResourceSize( const Resource& r )
{
    int resNum = m_pZipFile->Find( r.m_name.c_str() );
    if ( resNum == -1 ) {
        return -1;
    }

    return m_pZipFile->GetFileLen( resNum );
}

int ResourceZipFile::VGetRawResource( const Resource& r, char* buffer )
{
    int size = 0;
    //std::optional<int> resNum = m_pZipFile->Find( r.m_name.c_str() );
    int resNum = m_pZipFile->Find( r.m_name.c_str() );
    if ( resNum != -1 ) {
        size = m_pZipFile->GetFileLen( resNum );
        m_pZipFile->ReadFile( resNum, buffer );
    }
    return size;
}

int ResourceZipFile::VGetNumResources() const
{
    return (m_pZipFile == nullptr) ? 
                         0 : 
                         m_pZipFile->GetNumFiles();
}

std::string ResourceZipFile::VGetResourceName( int num ) const
{
    std::string resName = "";
    if ( m_pZipFile != nullptr && 
         num >= 0 && 
         num < m_pZipFile->GetNumFiles() )
    {
        resName = m_pZipFile->GetFilename( num );
    }
    
    return resName;
}

