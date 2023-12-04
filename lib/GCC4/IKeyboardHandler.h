#ifndef GCC4_IKEYBOARD_HANDLER_H_INCLUDED
#define GCC4_IKEYBOARD_HANDLER_H_INCLUDED

class IKeyboardHandler
{
public:
    virtual bool VOnKeyDown(const char c) = 0;
    virtual bool VOnKeyUp(const char c) = 0;
};

#endif // GCC4_IKEYBOARD_HANDLER_H_INCLUDED
