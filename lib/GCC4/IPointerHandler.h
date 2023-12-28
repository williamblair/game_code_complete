#ifndef GCC4_IPOINTER_HANDLER_H_INCLUDED
#define GCC4_IPOINTER_HANDLER_H_INCLUDED

#include <string>

#include "Point.h"

class IPointerHandler
{
public:
    virtual bool VOnPointerMove(const Point& pos, const int radius) = 0;
    virtual bool VOnPointerButtonDown(const Point& pos, const int radius, const std::string& buttonName) = 0;
    virtual bool VOnPointerButtonUp(const Point& pos, const int radius, const std::string& buttonName) = 0;
};

#endif // GCC4_IPOINTER_HANDLER_H_INCLUDED
