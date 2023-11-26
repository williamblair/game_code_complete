#include <thread>
#include <chrono>

#include <GCC4/IPacket.h>
#include <GCC4/ResCache.h>
#include <GCC4/GameCodeApp.h>
#include <GCC4/SocketManager.h>

#define PROGRAM_TIME_SECONDS 10 // how long this test program should run

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

void serverThreadFcn()
{
    NetListenSocket serverSock(6000); // tcp server 6000
    unsigned int clientIp = 0;
    
    auto timeStart = std::chrono::steady_clock::now();
    auto curTime = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
                curTime - timeStart).count() < 1000*PROGRAM_TIME_SECONDS)
    {
        curTime = std::chrono::steady_clock::now();
        
        // sockets set to non-blocking so will return immediately
        SOCKET serverAcceptSock = serverSock.AcceptConnection(&clientIp);
        if (serverAcceptSock != INVALID_SOCKET) {
            printf("Server accepted connection from 0x%x\n", clientIp);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }
}

void clientThreadFcn()
{
    NetSocket clientSock;
    bool connected = false;
    
    auto timeStart = std::chrono::steady_clock::now();
    auto curTime = std::chrono::steady_clock::now();
    
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
                curTime - timeStart).count() < 1000*PROGRAM_TIME_SECONDS)
    {
        curTime = std::chrono::steady_clock::now();
        
        if (!connected) {
            unsigned int ip = 0x7f000001; // 127.0.0.1
            unsigned int port = 6000;
            connected = clientSock.Connect(ip, port);
            if (connected) {
                printf("Client connect success\n");
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }
}

int main()
{
    if (!g_pSocketManager->Init()) { return 1; }
    std::thread serverThread(serverThreadFcn);
    std::thread clientThread(clientThreadFcn);
    
    printf("clientThread join\n");
    clientThread.join(); // wait for threads to exit
    printf("serverThread join\n");
    serverThread.join();
    
    return 0;
}
