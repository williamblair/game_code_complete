#ifndef GCC4_COLOR_H_INCLUDED
#define GCC4_COLOR_H_INCLUDED

struct Color
{
    float r,g,b,a;
    
    inline void Set(float _r, float _g, float _b, float _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
    
    Color& operator=(const Color& c) {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }
};

extern Color g_White;
extern Color g_Black;
extern Color g_Transparent;

#endif // GCC4_COLOR_H_INCLUDED

