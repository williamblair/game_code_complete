#ifndef GCC4_POINT_H_INCLUDED
#define GCC4_POINT_H_INCLUDED

struct Point
{
    int x,y;

    Point() :
        x(0),y(0)
    {}
    Point(int _x, int _y) :
        x(_x),y(_y)
    {}
    
    inline bool operator==(const Point& rhs) {
        return (x == rhs.x) && (y == rhs.y);
    }
    inline bool operator!=(const Point& rhs) {
        return !(*this == rhs);
    }
};

#endif // GCC4_POINT_H_INCLUDED
