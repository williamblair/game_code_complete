#include <iostream>
#include <Resource.h>
#include <ResCache.h>
#include <ResourceZipFile.h>
#include <ResHandle.h>

int main()
{
    ResourceZipFile* zipFile = new ResourceZipFile( "../08_zip/testfile.zip" ); // freed automatically by resCache
    ResCache resCache( 50, zipFile ); // 50 megabytes of cache

    if ( !resCache.Init() ) {
        std::cout << "Failed to init res cache" << std::endl;
        return 1;
    }

    Resource resource( "testfile.txt" );
    std::shared_ptr<ResHandle> testText = resCache.GetHandle( &resource );
    int size = testText->Size();
    char* testTextStr = (char*)testText->Buffer();

    std::cout << "Test Text:" << std::endl
              << testTextStr << std::endl;

    return 0;
}

