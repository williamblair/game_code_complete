#include <iostream>
#include "ScriptEventListenerMgr.h"

ScriptEventListenerMgr::~ScriptEventListenerMgr()
{
    for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it) {
        delete *it;
    }
    m_listeners.clear();
}

void ScriptEventListenerMgr::AddListener(ScriptEventListener* pListener)
{
    if (m_listeners.find(pListener) == m_listeners.end()) {
        m_listeners.insert( pListener );
    } else {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "Listener already in list" << std::endl;
    }
}

void ScriptEventListenerMgr::DestroyListener(ScriptEventListener* pListener)
{
    if (m_listeners.find(pListener) != m_listeners.end()) {
        m_listeners.erase(pListener);
    } else {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "Failed to find listener" << std::endl;
    }
}

