#ifndef GCC4_RECT_H_INCLUDED
#define GCC4_RECT_H_INCLUDED

struct Rect
{
    int x;
    int y;
    int width;
    int height;
    
    Rect(int _x, int _y, int _width, int _height) :
        x(_x), y(_y), width(_width), height(_height)
    {}
    Rect() :
        x(0), y(0), width(0), height(0)
    {}
};

#endif // GCC4_RECT_H_INCLUDED
