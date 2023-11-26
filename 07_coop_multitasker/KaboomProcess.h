#ifndef KABOOM_PROCESS_H_INCLUDED
#define KABOOM_PROCESS_H_INCLUDED

#include <cstdint>
#include <string>

#include <GCC4/Process.h>

class KaboomProcess : public Process
{
public:

    explicit KaboomProcess();

protected:

    virtual void VOnUpdate(uint64_t deltaMs);

private:
    
    std::string m_message;
};

#endif // KABOOM_PROCESS_H_INCLUDED

