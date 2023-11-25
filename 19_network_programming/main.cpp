#include <thread>
#include <chrono>

#include <IPacket.h>
#include <ResCache.h>
#include <GameCodeApp.h>
#include <SocketManager.h>

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
BaseSocketManager sockMgr; // internally sets g_pSocketManager

int main()
{
    if (!g_pSocketManager->Init()) { return 1; }
    NetListenSocket serverSock(6000); // tcp server 6000
    unsigned int clientIp = 0;
    
    auto timeStart = std::chrono::steady_clock::now();
    auto curTime = std::chrono::steady_clock::now();
    while ( std::chrono::duration_cast<std::chrono::milliseconds>(
                curTime - timeStart).count() < 1000*10 )
    {
        curTime = std::chrono::steady_clock::now();
        
        // sockets set to non-blocking so will return immediately
        SOCKET serverAcceptSock = serverSock.AcceptConnection(&clientIp);
        if (serverAcceptSock != INVALID_SOCKET) {
            printf("Server accepted connection from 0x%x\n", clientIp);
        }
        
        std::this_thread::sleep_for( std::chrono::milliseconds(1000/60) );
    }
    
    return 0;
}
