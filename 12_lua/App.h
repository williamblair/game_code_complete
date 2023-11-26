#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <GCC4/EventManager.h>
#include <GCC4/ProcessManager.h>
#include <GCC4/ResCache.h>
#include <GCC4/ResourceZipFile.h>
#include <GCC4/GameCodeApp.h>

class LuaTestApp : public GameCodeApp
{
public:

    LuaTestApp();
    ~LuaTestApp();

    virtual const char* VGetGameTitle() { return "LuaTestApp"; }
    virtual const char* VGetGameAppDirectory() { return "./"; }

    virtual bool Init(const char* title, int screenWidth, int screenHeight);

    void Run();

    ResCache* m_pResCache;
private:
    ResourceZipFile* m_pZipFile;
    std::unique_ptr<EventManager> m_pEvtMgr;

    EventListenerDelegate m_fromLuaTestEvtDelegate;
    void FromLuaTestHandler(IEventDataPtr pEventData);
};

#endif // APP_H_INCLUDED

