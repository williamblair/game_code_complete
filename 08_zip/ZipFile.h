#ifndef ZIP_FILE_H_INCLUDED
#define ZIP_FILE_H_INCLUDED

#include <cstdio>
#include <memory>
#include <string>

class ZipFile
{
public:

    typedef std::map<std::string, int> ZipContentsMap;

    ZipFile();
    ~ZipFile();

    bool Init( const std::string& zipFileName );
    void End();

    int GetNumFiles() const { return m_nEntries; }

    /**
     * input is a dir entry number for the compressed file within the zip
     */ 
    std::string GetFilename( int i ) const;
    int GetFileLen( int i ) const;
    bool ReadFile( int i, void* pBuf );

    // TODO
    //bool ReadLargeFile( int i, void* pBuf, void (*progressCallback)(int, bool&) );
    
    /**
     * Returns a dir entry number for the given internal filename
     */ 
    int Find( const std::string& path ) const;

    ZipContentsMap m_zipContentsMap;

private:

    // type declarations
    struct TZipDirHeader;
    struct TZipDirFileHeader;
    struct TZipLocalHeader; 

    FILE* m_pFile;
    char* m_pDirData;
    int   m_nEntires;

    // pointers to the dir entries in pDirData
    const TZipDirFileHeader** m_ppDir;
};

#endif // ZIP_FILE_H_INCLUDED

