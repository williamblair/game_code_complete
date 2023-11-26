#ifndef GCC4_SCRIPT_EVENT_LISTENER_MGR_H_INCLUDED
#define GCC4_SCRIPT_EVENT_LISTENER_MGR_H_INCLUDED

#include <set>

#include "ScriptEventListener.h"

class ScriptEventListenerMgr
{
public:
    ~ScriptEventListenerMgr();
    void AddListener(ScriptEventListener* pListener);
    void DestroyListener(ScriptEventListener* pListener);

private:
    typedef std::set<ScriptEventListener*> ScriptEventListenerSet;
    ScriptEventListenerSet m_listeners;
};

#endif // GCC4_SCRIPT_EVENT_LISTENER_MGR_H_INCLUDED

