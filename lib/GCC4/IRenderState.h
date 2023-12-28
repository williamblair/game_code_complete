#ifndef GCC4_IRENDERSTATE_H_INCLUDED
#define GCC4_IRENDERSTATE_H_INCLUDED

#include <string>

class IRenderState
{
public:
    virtual std::string VToString() = 0;
};

#endif // GCC4_IRENDERSTATE_H_INCLUDED
