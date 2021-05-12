#include <cstring>
#include <cstdint>
#include <zlib.h>
#include <ZipFile.h>

// type definitions
struct __attribute__((packed)) ZipFile::TZipLocalHeader 
{
    enum
    {
        SIGNATURE = 0x04034b50
    };
    uint32_t sig;
    uint16_t version;
    uint16_t flag;
    uint16_t compression;
    uint16_t modTime;
    uint16_t modDate;
    uint32_t crc32;
    uint32_t cSize;     // compressed size
    uint32_t ucSize;    // uncompressed size
    uint16_t fnameLen;  // filename string follows header
    uint16_t xtraLen;   // extra field follows filename
};

struct __attribute__((packed)) ZipFile::TZipDirHeader
{
    enum
    {
        SIGNATURE = 0x06054b50
    };
    uint32_t sig;
    uint16_t nDisk;
    uint16_t nStartDisk;
    uint16_t nDirEntries;
    uint16_t totalDirEntries;
    uint32_t dirSize;
    uint32_t dirOffset;
    uint16_t cmntLen;
};

struct __attribute__((packed)) ZipFile::TZipDirFileHeader
{
    enum
    {
        SIGNATURE = 0x02014b50
    };
    uint32_t sig;
    uint16_t verMade;
    uint16_t verNeeded;
    uint16_t flag;
    uint16_t compression;
    uint16_t modTime;
    uint16_t modDate;
    uint32_t crc32;
    uint32_t cSize;       // compressed size
    uint32_t ucSize;      // uncompressed size
    uint16_t fnameLen;
    uint16_t xtraLen;
    uint16_t cmntLen;     // comment field
    uint16_t diskStart;
    uint16_t intAttr;
    uint32_t extAttr;
    uint32_t hdrOffset;

    char* GetName() const    { return (char*)(this + 1);    }
    char* GetExtra() const   { return GetName() + fnameLen; }
    char* GetComment() const { return GetExtra() + xtraLen; }
}

ZipFile::ZipFile()
{
    m_nEntries = 0;
    m_pFile = nullptr;
    m_pDirData = nullptr;
}

ZipFile::~ZipFile()
{
    End();
    if ( m_pFile )
    {
        fclose( m_pFile );
    }
}

bool ZipFile::Init( const std::string& zipFileName )
{
    End();
    m_pFile = fopen( zipFileName, "rb" );
    if ( !m_pFile ) {
        printf( "Failed to open zip file: %s\n", zipFileName );
        return false;
    }

    // assuming no extra comment at end, read the whole end record
    TZipDirHeader dh;

    fseek( m_pFile, -(int)sizeof(dh), SEEK_END );
    long dhOffset = ftell( m_pFile );
    memset( (void*)&dh, 0, sizeof(dh) );
    fread( (void*)&dh, sizeof(dh), 1, m_pFile );

    // verify dir header signature
    if ( dh.sig != TZipDirHeader::SIGNATURE )
    {
        printf(  "Zip file bad dir header signature\n" );
        return false;
    }

    // move file to beginning of directory
    fseek( m_pFile, dhOffset - dh.dirSize, SEEK_SET );

    // allocate buffer to store all compressed files
    m_pDirData = new char[ dh.dirSize + dh.nDirEntries*sizeof( *m_ppDir ) ];
    if ( !m_pDirData ) {
        printf( "Failed to allocate zip data\n" );
        return false;
    }
    memset( (void*)m_pDirData, 0, dh.dirSize + dh.nDirEntries*sizeof( *m_ppDir ) );
    
    // read in the directory
    fread( (void*)m_pDirData, dh.dirSize, 1, m_pFile );
}

