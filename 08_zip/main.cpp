#include <cstdio>

#include <GCC4/ZipFile.h>

#include <GCC4/ResCache.h>
#include <GCC4/GameCodeApp.h>

// Required globals - TODO move
ResCache* g_ResCache = nullptr;
class TestGCCApp : public GameCodeApp
{
public:
    virtual const char* VGetGameTitle() { return "TestGCCApp"; }
    virtual const char* VGetGameAppDirectory() { return "./"; }
};
TestGCCApp g_TestGCCApp;
GameCodeApp* g_pApp = &g_TestGCCApp;

int main()
{
    ZipFile zipFile;
    
    if (!zipFile.Init("testfile.zip")) {
        printf( "Failed to init zip file\n" );
        return 1;
    } 
    printf("Successfully init zip file; dirs:\n");
    
    for (auto it = zipFile.m_zipContentsMap.begin();
        it != zipFile.m_zipContentsMap.end();
        ++it)
    {
        printf("%s: %d\n", it->first.c_str(), it->second);
    }

    int textFileDir = zipFile.Find("testfile.txt");
    if (textFileDir < 0) {
        printf( "Failed to find testfile.txt in zip\n" );
        return 1;
    }

    char* textFileBuf = nullptr;
    int textFileLen = zipFile.GetFileLen( textFileDir );
    if (textFileLen <= 0) {
        printf("Failed to get textfile.txt size\n");
        return 1;
    }
    textFileBuf = new char[textFileLen+1];

    if (!zipFile.ReadFile(textFileDir, (void*)textFileBuf)) {
        printf( "Failed to uncompress textfile.txt\n" );
        delete[] textFileBuf;
        return 1;
    }
    textFileBuf[textFileLen] = '\0';

    printf("\ntestfile.txt contents:\n%s\n", textFileBuf);

    delete[] textFileBuf;

    return 0;
}

