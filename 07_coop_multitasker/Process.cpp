#include <Process.h>

Process::Process()
{
}

Process::~Process()
{
    if ( m_pChild )
    {
        m_pChild->VOnAbort();
    }
}


StrongProcessPtr Process::RemoveChild()
{
    if ( m_pChild )
    {
        // prevents child from being destroyed when we reset m_pChild
        StrongProcessPtr pChild = m_pChild; 
        m_pChild.reset();
        return pChild;
    }

    return StrongProcessPtr();
}


