#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED

struct Vec3
{
    float x, y, z;

    Vec3() :
        x(0.0f),
        y(0.0f),
        z(0.0f)
    {}

    Vec3( float _x, float _y, float _z ) :
        x(_x),
        y(_y),
        z(_z)
    {}
};

#endif // VEC3_H_INCLUDED

