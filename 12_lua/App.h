#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <GCC4/EventManager.h>
#include <GCC4/ProcessManager.h>
#include <GCC4/ResCache.h>
#include <GCC4/ResourceZipFile.h>

class App
{
public:

    App();
    ~App();

    bool Init();

    void Run();

    ResCache* m_pResCache;
    ProcessManager* m_pProcessMgr;
private:
    ResourceZipFile* m_pZipFile;
    std::unique_ptr<EventManager> m_pEvtMgr;

    EventListenerDelegate m_fromLuaTestEvtDelegate;
    void FromLuaTestHandler( IEventDataPtr pEventData );
};

extern App* g_pTestApp; // global instance

#endif // APP_H_INCLUDED

