#include <cstring>
#include <cstdint>
#include <algorithm>
#include <zlib.h>
#include <ZipFile.h>

// https://docs.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=cmd
#define _MAX_PATH 260 // windows max path equivalent

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
};

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
    m_pFile = fopen( zipFileName.c_str(), "rb" );
    if ( !m_pFile ) {
        printf( "Failed to open zip file: %s\n", zipFileName.c_str() );
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

    // process each entry
    char* pfh = m_pDirData;
    m_ppDir = (const TZipDirFileHeader**)(m_pDirData + dh.dirSize);

    bool success = true;
    for ( int i = 0; i < int(dh.nDirEntries) && success; ++i )
    {
        TZipDirFileHeader& fh = *(TZipDirFileHeader*)pfh;

        // save address of the nth file
        m_ppDir[i] = &fh;

        if ( fh.sig != TZipDirFileHeader::SIGNATURE )
        {
            success = false;
        }
        else
        {
            pfh += sizeof( fh );

            // convert UNIX slashes to DOS backlashes
            for ( int j = 0; j < fh.fnameLen; ++j )
            {
                if ( pfh[j] == '/' )
                {
                    pfh[j] = '\\';
                }
            }

            char fileName[_MAX_PATH];
            memcpy( (void*)fileName, (void*)pfh, fh.fnameLen );
            fileName[fh.fnameLen] = 0; // null terminate string
            std::string spath = std::string( fileName );
            std::string spathLowercase = spath;
            std::transform( spath.begin(), // convert to lowercase
                            spath.end(), 
                            spathLowercase.begin(), 
                            (int(*)(int))std::tolower );
            m_zipContentsMap[ spathLowercase ] = i;

            // skip name, extra, and comment fields
            pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
        }
    }
    if ( !success )
    {
        if ( m_pDirData )
        {
            delete[] m_pDirData;
            m_pDirData = nullptr;
        }
    }
    else
    {
        m_nEntries = dh.nDirEntries;
    }

    return success;
}

int ZipFile::Find( const std::string& path ) const
{
    std::string lowerCase = path;
    std::transform( path.begin(),
                    path.end(),
                    lowerCase.begin(),
                    (int(*)(int))std::tolower );
    ZipContentsMap::const_iterator i = m_zipContentsMap.find( lowerCase );
    if ( i == m_zipContentsMap.end() )
    {
        return -1;
    }

    return i->second;
}

void ZipFile::End()
{
    m_zipContentsMap.clear();
    if ( m_pDirData )
    {
        delete[] m_pDirData;
        m_pDirData = nullptr;
    }
    m_nEntries = 0;
}

std::string ZipFile::GetFilename( int i ) const
{
    std::string fileName = "";
    if ( i > 0 && i < m_nEntries )
    {
        char pszDest[ _MAX_PATH ];
        memcpy( pszDest, m_ppDir[i]->GetName(), m_ppDir[i]->fnameLen );
        pszDest[ m_ppDir[i]->fnameLen ] = 0; // null terminate
        fileName = std::string( pszDest );
    }
    return fileName;
}

int ZipFile::GetFileLen( int i ) const
{
    if ( i < 0 || i >= m_nEntries ) {
        return -1;
    }
    return m_ppDir[i]->ucSize;
}

bool ZipFile::ReadFile( int i, void* pBuf )
{
    if ( pBuf == nullptr || i < 0 || i >= m_nEntries )
    {
        return false;
    }

    // go to compressed file location within FILE*
    fseek( m_pFile, m_ppDir[i]->hdrOffset, SEEK_SET );
    
    TZipLocalHeader h;
    memset( (void*)&h, 0, sizeof(h) );
    fread( (void*)&h, sizeof(h), 1, m_pFile );
    if ( h.sig != TZipLocalHeader::SIGNATURE )
    {
        printf( "ReadFile: invalid local file header signature\n" );
        return false;
    }

    // skip extra fields
    fseek( m_pFile, h.fnameLen + h.xtraLen, SEEK_CUR );

    // ZLib value
    if ( h.compression == Z_NO_COMPRESSION )
    {
        // simply read the raw data
        fread( (void*)pBuf, h.cSize, 1, m_pFile );
        return true;
    }
    // only deflated alg supported
    else if ( h.compression != Z_DEFLATED )
    {
        return false;
    }

    // create compressed data buffer
    char* pcData = new char[ h.cSize ];
    if ( pcData == nullptr )
    {
        printf( "Failed to alloc compressed data buff\n" );
        return false;
    }

    // read the compressed file data
    memset( (void*)pcData, 0, h.cSize );
    fread( (void*)pcData, h.cSize, 1, m_pFile );

    bool ret = true;

    // set up zlib inflate stream
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)pcData;
    stream.avail_in = (uInt)h.cSize;
    stream.next_out = (Bytef*)pBuf;
    stream.avail_out = h.ucSize;
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    // wbits < 0 indicates no zlib header inside the data
    err = inflateInit2( &stream, -MAX_WBITS );
    if ( err == Z_OK )
    {
        err = inflate( &stream, Z_FINISH );
        inflateEnd( &stream );
        if ( err == Z_STREAM_END )
        {
            err = Z_OK;
        }
        inflateEnd( &stream );
    }
    if ( err != Z_OK )
    {
        printf( "zlib inflation decompression error\n" );
        ret = false;
    }

    delete[] pcData;
    return ret;
}

