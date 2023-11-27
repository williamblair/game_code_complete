#ifndef GCC4_CRITICAL_SECTION_H_INCLUDED
#define GCC4_CRITICAL_SECTION_H_INCLUDED

#include <mutex>

// TODO - make noncopyable
class CriticalSection /*: public GCC_noncopyable*/
{
public:
    CriticalSection() {}
    ~CriticalSection() {}

    void Lock() { m_mutex.lock(); }
    void Unlock() { m_mutex.unlock(); }

protected:
    std::mutex m_mutex;
};

// TODO - make noncopyable
class ScopedCriticalSection/* : public GCC_noncopyable*/
{
public:
    ScopedCriticalSection(CriticalSection& csResource) :
        m_csResource(csResource)
    {
        m_csResource.Lock();
    }
    ~ScopedCriticalSection()
    {
        m_csResource.Unlock();
    }
private:
    CriticalSection& m_csResource;
};

#endif // GCC4_CRITICAL_SECTION_H_INCLUDED

