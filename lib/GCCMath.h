#ifndef GCC4MATH_CPP_H_INCLUDED
#define GCC4MATH_CPP_H_INCLUDED

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.141592653589793f
#endif

// forward declarations
struct Vec3;
struct Mat4x4;

// globals
extern Vec3 g_Forward;
extern Vec3 g_Right;
extern Vec3 g_Up;

struct Vec2
{
    union
    {
        struct { float x, y; };
        struct { float u, v; };
    };

    Vec2() :
        x(0.0f),
        y(0.0f)
    {}

    Vec2(float x, float y) :
        x(x),
        y(y)
    {}

    Vec2(const Vec2& other) :
        x(other.x),
        y(other.y)
    {}

    Vec2& operator=(const Vec2& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    Vec2& operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

inline Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(
        lhs.x+rhs.x,
        lhs.y+rhs.y
    );
}
inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(
        lhs.x-rhs.x,
        lhs.y-rhs.y
    );
}
inline Vec2 operator*(const Vec2& lhs, const float rhs) {
    return Vec2(
        lhs.x * rhs,
        lhs.y * rhs
    );
}
inline Vec2 operator*(const float& lhs, const Vec2& rhs) {
    return rhs * lhs;
}
inline Vec2 Normalize(const Vec2& v) {
    const float mag = sqrtf(v.x*v.x + v.y*v.y);
    return Vec2(
        v.x / mag,
        v.y / mag
    );
}
inline bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return (fabsf(lhs.x - rhs.x) < 0.0001f) &&
        (fabsf(lhs.y - rhs.y) < 0.0001f);
}


struct Vec3
{
    union
    {
        struct { float x,y,z; };
        float v[3];
    };

    Vec3() :
        x(0.0f),
        y(0.0f),
        z(0.0f)
    {}

    Vec3(float x, float y, float z) :
        x(x),
        y(y),
        z(z)
    {}

    Vec3(const Vec3& other) :
        x(other.x),
        y(other.y),
        z(other.z)
    {}

    Vec3& operator=(const Vec3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        
        return *this;
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }
    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }
};

struct Vec4
{
    union
    {
        struct { float x,y,z,w; };
        float v[4];
    };

    Vec4() :
        x(0.0f),
        y(0.0f),
        z(0.0f),
        w(1.0f)
    {}

    Vec4(float x, float y, float z, float w) :
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    Vec4(const Vec4& other) :
        x(other.x),
        y(other.y),
        z(other.z),
        w(other.w)
    {}

    Vec4(const Vec3& vec, const float w) :
        x(vec.x),
        y(vec.y),
        z(vec.z),
        w(w)
    {}

    Vec4& operator=(const Vec4& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        
        return *this;
    }

    Vec4& operator+=(const Vec4& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;

        return *this;
    }
    Vec4& operator-=(const Vec4& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;

        return *this;
    }
};

inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z
    );
}
inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z
    );
}
inline Vec3 operator*(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z
    );
}
inline Vec3 operator*(const Vec3& v, const float scale) {
    return Vec3(
        v.x * scale,
        v.y * scale,
        v.z * scale
    );
}
inline Vec3 operator*(const float scale, const Vec3& v) {
    return Vec3(
        v.x * scale,
        v.y * scale,
        v.z * scale
    );
}
inline bool operator==(const Vec3& lhs, const Vec3& rhs) {
    return (fabsf(lhs.x - rhs.x) < 0.0001f) &&
        (fabsf(lhs.y - rhs.y) < 0.0001f) &&
        (fabsf(lhs.z - rhs.z) < 0.0001f);
}
inline bool operator!=(const Vec3& lhs, const Vec3& rhs) {
    return !(lhs == rhs);
}
inline float MagSqr(const Vec3& v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}
inline float Mag(const Vec3& v) {
    return sqrtf(MagSqr(v));
}
inline float Dot(const Vec3& lhs, const Vec3& rhs) {
    return (lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z);
}
inline Vec3 Cross(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(
        lhs.y*rhs.z - lhs.z*rhs.y,
        lhs.z*rhs.x - lhs.x*rhs.z,
        lhs.x*rhs.y - lhs.y*rhs.x
    );
}
inline Vec3 Normalize(const Vec3& v) {
    const float magInv = 1.0f / sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    return Vec3(
        v.x * magInv,
        v.y * magInv,
        v.z * magInv
    );
}
inline Vec3 Lerp(const Vec3& lhs, const Vec3& rhs, const float t) {
    return ((1.0f - t)*lhs) + (t*rhs);
}

inline bool operator==(const Vec4& lhs, const Vec4& rhs) {
    return (fabsf(lhs.x - rhs.x) < 0.0001f) &&
        (fabsf(lhs.y - rhs.y) < 0.0001f) &&
        (fabsf(lhs.z - rhs.z) < 0.0001f) &&
        (fabsf(lhs.w - rhs.w) < 0.0001f);
}

struct Quat
{
    union
    {
        struct { float x,y,z,w; };
        float data[4];
    };

    Quat() :
        x(0.0f),
        y(0.0f),
        z(0.0f),
        w(1.0f)
    {}

    Quat(float x, float y, float z, float w) :
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    Quat(const float angleRadians, const Vec3& axis)
    {
        const float sinHalf = sinf(angleRadians * 0.5f);
        const float cosHalf = cosf(angleRadians * 0.5f);
        x = axis.x * sinHalf;
        y = axis.y * sinHalf;
        z = axis.z * sinHalf;
        w = cosHalf;
    }

    Quat& operator=(const Quat& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;

        return *this;
    }
};

inline Quat operator*(const Quat& a, const Quat& b) {
    /*Vec3 av(a.x, a.y, a.z);
    Vec3 bv(b.x, b.y, b.z);
    Vec3 v = Cross(av, bv) + (bv*a.w) + (av*b.w);
    float w = a.w*b.w - Dot(av, bv);
    return Quat(
        v.x,
        v.y,
        v.z,
        w
    );*/
    return Quat(
        b.x*a.w + b.y*a.z - b.z*a.y + b.w*a.x,
        -b.x*a.z + b.y*a.w + b.z*a.x + b.w*a.y,
        b.x*a.y - b.y*a.x + b.z*a.w + b.w*a.z,
        -b.x*a.x - b.y*a.y - b.z*a.z + b.w*a.w
    );
}
inline Quat operator*(const Quat& a, const float t) {
    return Quat(
        a.x * t,
        a.y * t,
        a.z * t,
        a.w * t
    );
}
inline Quat operator+(const Quat& a, const Quat& b) {
    return Quat(
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    );
}
inline Quat operator-(const Quat& a, const Quat& b) {
    return Quat(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    );
}

inline Quat operator-(const Quat& a) {
    return Quat(-a.x, -a.y, -a.z, -a.w);
}

inline bool operator==(const Quat& lhs, const Quat& rhs) {
    return (fabsf(lhs.x - rhs.x) < 0.000001f) &&
        (fabsf(lhs.y - rhs.y) < 0.000001f) &&
        (fabsf(lhs.z - rhs.z) < 0.000001f) &&
        (fabsf(lhs.w - rhs.w) < 0.000001f);
}
inline bool operator!=(const Quat& lhs, const Quat& rhs) {
    return !(lhs == rhs);
}

// magnitude/length squared
inline float MagSqr(const Quat& q) {
    return q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
}

// magnitude/length
inline float Mag(const Quat& q) {
    return sqrtf(MagSqr(q));
}

inline float Dot(const Quat& lhs, const Quat& rhs) {
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
}

inline Quat Normalize(const Quat& q) {
    float lenInv = 1.0f / Mag(q);
    return Quat(
        q.x * lenInv,
        q.y * lenInv,
        q.z * lenInv,
        q.w * lenInv
    );
}

inline Quat Lerp(const Quat& lhs, const Quat& rhs, const float t) {
    return lhs + (rhs - lhs)*t;
}
inline Quat NLerp(const Quat& lhs, const Quat& rhs, const float t) {
    return Normalize(lhs + (rhs - lhs)*t);
}

// conjugate
inline Quat Conj(const Quat& q) {
    return Quat(
        -q.x,
        -q.y,
        -q.z,
        q.w
    );
}
// inverse
inline Quat Inverse(const Quat& q) {
    float magSqr = MagSqr(q);
    if (magSqr < 0.000001f) {
        return Quat();
    }
    float invMagSqr = 1.0f / magSqr;
    return Quat(
        -q.x * invMagSqr,
        -q.y * invMagSqr,
        -q.z * invMagSqr,
        q.w * invMagSqr
    );
}

// rotates v by q
// assumes q is normalized
inline Vec3 Transform(const Quat& q, const Vec3& v) {
    Vec3 u(q.x, q.y, q.z);
    return 
        (2.0f * Dot(u, v) * u +
        (q.w*q.w - Dot(u, u)) * v +
        2.0f * q.w * Cross(u, v));
}

// forward declaration
inline Vec3 Transform(const Mat4x4& m, const Vec3& v, const float w = 1.0f);
struct Mat4x4
{
    // Column major ordering
    union
    {
        float v[4][4];
        struct { float r0c0, r1c0, r2c0, r3c0,
                       r0c1, r1c1, r2c1, r3c1,
                       r0c2, r1c2, r2c2, r3c2,
                       r0c3, r1c3, r2c3, r3c3; };
    };

    static Mat4x4 g_Identity;

    Mat4x4() :
        r0c0(1.0f), r1c0(0.0f), r2c0(0.0f), r3c0(0.0f),
        r0c1(0.0f), r1c1(1.0f), r2c1(0.0f), r3c1(0.0f),
        r0c2(0.0f), r1c2(0.0f), r2c2(1.0f), r3c2(0.0f),
        r0c3(0.0f), r1c3(0.0f), r2c3(0.0f), r3c3(1.0f)
    {
    }

    Mat4x4(const Mat4x4& other) :
        r0c0(other.r0c0), r1c0(other.r1c0), r2c0(other.r2c0), r3c0(other.r3c0),
        r0c1(other.r0c1), r1c1(other.r1c1), r2c1(other.r2c1), r3c1(other.r3c1),
        r0c2(other.r0c2), r1c2(other.r1c2), r2c2(other.r2c2), r3c2(other.r3c2),
        r0c3(other.r0c3), r1c3(other.r1c3), r2c3(other.r2c3), r3c3(other.r3c3)
    {
    }

    Mat4x4(float r0c0, float r0c1, float r0c2, float r0c3,
         float r1c0, float r1c1, float r1c2, float r1c3,
         float r2c0, float r2c1, float r2c2, float r2c3,
         float r3c0, float r3c1, float r3c2, float r3c3) :
        r0c0(r0c0), r1c0(r1c0), r2c0(r2c0), r3c0(r3c0),
        r0c1(r0c1), r1c1(r1c1), r2c1(r2c1), r3c1(r3c1),
        r0c2(r0c2), r1c2(r1c2), r2c2(r2c2), r3c2(r3c2),
        r0c3(r0c3), r1c3(r1c3), r2c3(r2c3), r3c3(r3c3)
    {}

    inline float operator()(int row, int col) const {
        return v[col][row];
    }

    Mat4x4& operator=(const Mat4x4& other) {
        r0c0 = other.r0c0;
        r0c1 = other.r0c1;
        r0c2 = other.r0c2;
        r0c3 = other.r0c3;
        
        r1c0 = other.r1c0;
        r1c1 = other.r1c1;
        r1c2 = other.r1c2;
        r1c3 = other.r1c3;
        
        r2c0 = other.r2c0;
        r2c1 = other.r2c1;
        r2c2 = other.r2c2;
        r2c3 = other.r2c3;
        
        r3c0 = other.r3c0;
        r3c1 = other.r3c1;
        r3c2 = other.r3c2;
        r3c3 = other.r3c3;
        
        return *this;
    }
    
    Mat4x4& operator+=(const Mat4x4& other) {
        r0c0 += other.r0c0;
        r0c1 += other.r0c1;
        r0c2 += other.r0c2;
        r0c3 += other.r0c3;
        
        r1c0 += other.r1c0;
        r1c1 += other.r1c1;
        r1c2 += other.r1c2;
        r1c3 += other.r1c3;
        
        r2c0 += other.r2c0;
        r2c1 += other.r2c1;
        r2c2 += other.r2c2;
        r2c3 += other.r2c3;
        
        r3c0 += other.r3c0;
        r3c1 += other.r3c1;
        r3c2 += other.r3c2;
        r3c3 += other.r3c3;
        
        return *this;
    }
    
    inline Vec3 GetPosition() const {
        return Vec3(r0c3, r1c3, r2c3);
    }
    inline void SetPosition(const Vec3& pos) {
        r0c3 = pos.x;
        r1c3 = pos.y;
        r2c3 = pos.z;
    }
    
    inline Vec3 GetDirection() const {
        Mat4x4 justRot = *this;
        // set translation to 0
        justRot.r0c3 = 0.0f;
        justRot.r1c3 = 0.0f;
        justRot.r2c3 = 0.0f;
        Vec3 forward = Transform(justRot, g_Forward);
        return forward;
    }
};

inline Mat4x4 operator*(const Mat4x4& lhs, const Mat4x4& rhs) {
    Vec4 row0(
        lhs(0,0)*rhs(0,0) + lhs(0,1)*rhs(1,0) + lhs(0,2)*rhs(2,0) + lhs(0,3)*rhs(3,0),
        lhs(0,0)*rhs(0,1) + lhs(0,1)*rhs(1,1) + lhs(0,2)*rhs(2,1) + lhs(0,3)*rhs(3,1),
        lhs(0,0)*rhs(0,2) + lhs(0,1)*rhs(1,2) + lhs(0,2)*rhs(2,2) + lhs(0,3)*rhs(3,2),
        lhs(0,0)*rhs(0,3) + lhs(0,1)*rhs(1,3) + lhs(0,2)*rhs(2,3) + lhs(0,3)*rhs(3,3)
    );
    Vec4 row1(
        lhs(1,0)*rhs(0,0) + lhs(1,1)*rhs(1,0) + lhs(1,2)*rhs(2,0) + lhs(1,3)*rhs(3,0),
        lhs(1,0)*rhs(0,1) + lhs(1,1)*rhs(1,1) + lhs(1,2)*rhs(2,1) + lhs(1,3)*rhs(3,1),
        lhs(1,0)*rhs(0,2) + lhs(1,1)*rhs(1,2) + lhs(1,2)*rhs(2,2) + lhs(1,3)*rhs(3,2),
        lhs(1,0)*rhs(0,3) + lhs(1,1)*rhs(1,3) + lhs(1,2)*rhs(2,3) + lhs(1,3)*rhs(3,3)
    );
    Vec4 row2(
        lhs(2,0)*rhs(0,0) + lhs(2,1)*rhs(1,0) + lhs(2,2)*rhs(2,0) + lhs(2,3)*rhs(3,0),
        lhs(2,0)*rhs(0,1) + lhs(2,1)*rhs(1,1) + lhs(2,2)*rhs(2,1) + lhs(2,3)*rhs(3,1),
        lhs(2,0)*rhs(0,2) + lhs(2,1)*rhs(1,2) + lhs(2,2)*rhs(2,2) + lhs(2,3)*rhs(3,2),
        lhs(2,0)*rhs(0,3) + lhs(2,1)*rhs(1,3) + lhs(2,2)*rhs(2,3) + lhs(2,3)*rhs(3,3)
    );
    Vec4 row3(
        lhs(3,0)*rhs(0,0) + lhs(3,1)*rhs(1,0) + lhs(3,2)*rhs(2,0) + lhs(3,3)*rhs(3,0),
        lhs(3,0)*rhs(0,1) + lhs(3,1)*rhs(1,1) + lhs(3,2)*rhs(2,1) + lhs(3,3)*rhs(3,1),
        lhs(3,0)*rhs(0,2) + lhs(3,1)*rhs(1,2) + lhs(3,2)*rhs(2,2) + lhs(3,3)*rhs(3,2),
        lhs(3,0)*rhs(0,3) + lhs(3,1)*rhs(1,3) + lhs(3,2)*rhs(2,3) + lhs(3,3)*rhs(3,3)
    );
    return Mat4x4(
        row0.x, row0.y, row0.z, row0.w,
        row1.x, row1.y, row1.z, row1.w,
        row2.x, row2.y, row2.z, row2.w,
        row3.x, row3.y, row3.z, row3.w
    );
}
inline Mat4x4 operator*(const Mat4x4& lhs, const float rhs) {
    return Mat4x4(
        lhs(0,0)*rhs, lhs(0,1)*rhs, lhs(0,2)*rhs, lhs(0,3)*rhs,
        lhs(1,0)*rhs, lhs(1,1)*rhs, lhs(1,2)*rhs, lhs(1,3)*rhs,
        lhs(2,0)*rhs, lhs(2,1)*rhs, lhs(2,2)*rhs, lhs(2,3)*rhs,
        lhs(3,0)*rhs, lhs(3,1)*rhs, lhs(3,2)*rhs, lhs(3,3)*rhs
    );
}
inline Mat4x4 operator*(const float lhs, const Mat4x4& rhs) {
    return rhs * lhs;
}
inline Vec4 operator*(const Mat4x4& lhs, const Vec4& rhs) {
    return Vec4(
        lhs(0,0)*rhs.x + lhs(0,1)*rhs.y + lhs(0,2)*rhs.z + lhs(0,3)*rhs.w,
        lhs(1,0)*rhs.x + lhs(1,1)*rhs.y + lhs(1,2)*rhs.z + lhs(1,3)*rhs.w,
        lhs(2,0)*rhs.x + lhs(2,1)*rhs.y + lhs(2,2)*rhs.z + lhs(2,3)*rhs.w,
        lhs(3,0)*rhs.x + lhs(3,1)*rhs.y + lhs(3,2)*rhs.z + lhs(3,3)*rhs.w
    );
}
inline Mat4x4 operator+(const Mat4x4& lhs, const Mat4x4& rhs) {
    return Mat4x4(
        lhs(0,0)+rhs(0,0), lhs(0,1)+rhs(0,1), lhs(0,2)+rhs(0,2), lhs(0,3)+rhs(0,3),
        lhs(1,0)+rhs(1,0), lhs(1,1)+rhs(1,1), lhs(1,2)+rhs(1,2), lhs(1,3)+rhs(1,3),
        lhs(2,0)+rhs(2,0), lhs(2,1)+rhs(2,1), lhs(2,2)+rhs(2,2), lhs(2,3)+rhs(2,3),
        lhs(3,0)+rhs(3,0), lhs(3,1)+rhs(3,1), lhs(3,2)+rhs(3,2), lhs(3,3)+rhs(3,3)
    );
}

inline float Deg2Rad(const float deg) { return deg * M_PI / 180.0f; }
inline float Rad2Deg(const float rad) { return rad * 180.0f / M_PI; }
inline float Clamp(float val, const float min, float max) {
    return (val < min ? min : 
        (val > max ? max : val));
}

// multiplies v by m
inline Vec3 Transform(const Mat4x4& m, const Vec3& v, const float w) {
    return Vec3(
        m(0,0)*v.x + m(0,1)*v.y + m(0,2)*v.z + m(0,3)*w,
        m(1,0)*v.x + m(1,1)*v.y + m(1,2)*v.z + m(1,3)*w,
        m(2,0)*v.x + m(2,1)*v.y + m(2,2)*v.z + m(2,3)*w
    );
}
inline Vec4 Transform(const Mat4x4& m, const Vec4& v) {
    return Vec4(
        m(0,0)*v.x + m(0,1)*v.y + m(0,2)*v.z + m(0,3)*v.w,
        m(1,0)*v.x + m(1,1)*v.y + m(1,2)*v.z + m(1,3)*v.w,
        m(2,0)*v.x + m(2,1)*v.y + m(2,2)*v.z + m(2,3)*v.w,
        m(3,0)*v.x + m(3,1)*v.y + m(3,2)*v.z + m(3,3)*v.w
    );
}

inline Mat4x4 Translate(const Vec3& trans) {
    return Mat4x4(
        1.0f, 0.0f, 0.0f, trans.x,
        0.0f, 1.0f, 0.0f, trans.y,
        0.0f, 0.0f, 1.0f, trans.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
inline Mat4x4 Translate(float x, float y, float z) {
    return Mat4x4(
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

inline Mat4x4 Rotate(const float angleRadians, const Vec3& axis) {
    const float c = cosf(angleRadians);
    const float s = sinf(angleRadians);
    
    const float axay = axis.x*axis.y;
    const float axaz = axis.x*axis.z;
    const float ayaz = axis.y*axis.z;
    const float ax2 = axis.x*axis.x;
    const float ay2 = axis.y*axis.y;
    const float az2 = axis.z*axis.z;

    return Mat4x4(
        c+(1-c)*ax2, (1-c)*axay-s*axis.z, (1-c)*axaz+s*axis.y, 0.0f,
        (1-c)*axay+s*axis.z, c+(1-c)*ay2, (1-c)*ayaz-s*axis.x, 0.0f,
        (1-c)*axaz-s*axis.y, (1-c)*ayaz+s*axis.x, c+(1-c)*az2, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

inline Mat4x4 Scale(const Vec3& s) {
    return Mat4x4(
        s.x, 0.0f, 0.0f, 0.0f,
        0.0f, s.y, 0.0f, 0.0f,
        0.0f, 0.0f, s.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
inline Mat4x4 Scale(float x, float y, float z) {
    return Mat4x4(
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

inline Mat4x4 QuatToMat4x4(const Quat& q) {
    const float xy = q.x*q.y;
    const float wz = q.w*q.z;
    const float wy = q.w*q.y;
    const float wx = q.w*q.x;
    const float xz = q.x*q.z;
    const float yz = q.y*q.z;
    const float twox2 = 2.0f*q.x*q.x;
    const float twoy2 = 2.0f*q.y*q.y;
    const float twoz2 = 2.0f*q.z*q.z;
    
    return Mat4x4(
        1-twoy2-twoz2, 2*(xy-wz), 2*(xz+wy), 0.0f,
        2*(xy+wz), 1-twox2-twoz2, 2*(yz-wx), 0.0f,
        2*(xz-wy), 2*(yz+wx), 1-twox2-twoy2, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

// rotation upon "from" in order to get to "to"
inline Quat FromTo(const Vec3& from, const Vec3& to) {
    Vec3 f = Normalize(from);
    Vec3 t = Normalize(to);
    if (f == t) {
        return Quat();
    }
    // opposition directions
    else if (f == (t * -1.0f)) {
        // the most orthogonal axis of from used to create quat
        Vec3 ortho = Vec3(1,0,0);
        if (fabsf(f.y) < fabsf(f.x)) {
            ortho = Vec3(0,1,0);
        }
        if (fabsf(f.z) < fabsf(f.y) &&
            fabsf(f.z) < fabsf(f.x))
        {
            ortho = Vec3(0,0,1);
        }
        
        Vec3 axis = Normalize(Cross(f, ortho));
        return Quat(axis.x, axis.y, axis.z, 0);
    }
    
    // sin of the half vector between the two (cross product) is the
    // axis of rotation, and cos of the half vector (dot product) is the angle
    Vec3 half = Normalize(f + t);
    Vec3 axis = Cross(f, half);
    return Quat(axis.x, axis.y, axis.z, Dot(f, half)); // dot == angle
}

// rotation for this direction
inline Quat LookRotation(const Vec3& direction, const Vec3& up) {
    // find orthonormal basis vectors
    Vec3 f = Normalize(direction); // forward
    Vec3 u = Normalize(up); // desired up
    Vec3 r = Cross(u, f); // right
    u = Cross(f, r); // object up
    
    // from world forward to object forward
    Quat worldToObject = FromTo(Vec3(0,0,1), f);
    
    // new object up direction
    Vec3 objectUp = Transform(worldToObject, Vec3(0,1,0));
    
    // new object up to desired up
    Quat u2u = FromTo(objectUp, u);
    
    // rotate forward direction then twist up
    Quat result = worldToObject * u2u;
    return Normalize(result);
}

inline Quat Mat4x4ToQuat(const Mat4x4& m) {
    // mat 2nd col = up
    Vec3 up(m.r0c1, m.r1c1, m.r2c1);
    up = Normalize(up);
    
    // mat 3rd col = forward
    Vec3 forward(m.r0c2, m.r1c2, m.r2c2);
    forward = Normalize(forward);
    
    Vec3 right = Cross(up, forward);
    up = Cross(forward, right);
    
    return LookRotation(forward, up);
}

// Foundations of Game Engine Development Vol. 1 pg. 49
inline Mat4x4 Inverse(const Mat4x4& m) {
    Vec3 a(m(0,0), m(1,0), m(2,0));
    Vec3 b(m(0,1), m(1,1), m(2,1));
    Vec3 c(m(0,2), m(1,2), m(2,2));
    Vec3 d(m(0,3), m(1,3), m(2,3));
    
    const float x = m(3,0);
    const float y = m(3,1);
    const float z = m(3,2);
    const float w = m(3,3);
    
    Vec3 s = Cross(a, b);
    Vec3 t = Cross(c, d);
    Vec3 u = y*a - x*b;
    Vec3 v = w*c - z*d;
    
    float detInv = 1.0f / (Dot(s, v) + Dot(t, u));
    Vec3 row0 = (Cross(b, v) + y*t);
    Vec3 row1 = (Cross(v, a) - x*t);
    Vec3 row2 = (Cross(d, u) + w*s);
    Vec3 row3 = (Cross(u, c) - z*s);
    
    return Mat4x4(
        row0.x, row0.y, row0.z, -Dot(b, t),
        row1.x, row1.y, row1.z, Dot(a, t),
        row2.x, row2.y, row2.z, -Dot(d, s),
        row3.x, row3.y, row3.z, Dot(c, s)
    ) * detInv;
}

inline Mat4x4 Transpose(const Mat4x4& m) {
    return Mat4x4(
        m(0,0), m(1,0), m(2,0), m(3,0),
        m(0,1), m(1,1), m(2,1), m(3,1),
        m(0,2), m(1,2), m(2,2), m(3,2),
        m(0,3), m(1,3), m(2,3), m(3,3)
    );
}

// https://www.geertarien.com/blog/2017/07/30/breakdown-of-the-lookAt-function-in-OpenGL/
inline Mat4x4 LookAt(const Vec3& eye, const Vec3& at, const Vec3& up) {
    Vec3 zaxis = Normalize(at - eye);
    Vec3 xaxis = Normalize(Cross(zaxis, up));
    Vec3 yaxis = Cross(xaxis, zaxis);
    
    zaxis = zaxis * -1.0f;
    
    return Mat4x4(
        xaxis.x, xaxis.y, xaxis.z, -Dot(xaxis, eye),
        yaxis.x, yaxis.y, yaxis.z, -Dot(yaxis, eye),
        zaxis.x, zaxis.y, zaxis.z, -Dot(zaxis, eye),
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

// https://github.com/ps3dev/PSL1GHT/blob/master/common/vectormath/ppu/cpp/mat_aos.h
inline Mat4x4 Perspective(const float fovRad, const float aspect, const float near, const float far) {
    Mat4x4 result;
    float f, rangeInv;
    Vec4 zero, col0, col1, col2, col3;
    Vec4 tmp;
    f = tanf((M_PI/2.0f) - fovRad * 0.5f);
    rangeInv = 1.0f / (near - far);
    zero = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    tmp = zero;
    tmp.v[0] = f / aspect;
    col0 = tmp;
    tmp = zero;
    tmp.v[1] = f;
    col1 = tmp;
    tmp = zero;
    tmp.v[2] = (near + far) * rangeInv;
    tmp.v[3] = -1.0f;
    col2 = tmp;
    tmp = zero;
    tmp.v[2] = near * far * rangeInv * 2.0f;
    col3 = tmp;
    //result.col0 = col0;
    //result.col1 = col1;
    //result.col2 = col2;
    //result.col3 = col3;
    //return result;
    return Mat4x4(
        col0.x, col1.x, col2.x, col3.x,
        col0.y, col1.y, col2.y, col3.y,
        col0.z, col1.z, col2.z, col3.z,
        col0.w, col1.w, col2.w, col3.w
    );
}

// adapted from raylib rcore.c
inline Vec2 WorldToScreenCoords(Vec3& pos, Mat4x4& viewMat, Mat4x4& projMat, int scrWidth, int scrHeight) {
    Mat4x4 viewProj = projMat * viewMat;
    // we want to calculate and include the w coord
    Vec4 worldPos = viewProj * Vec4(pos, 1.0f);
    float w = worldPos.w;
    if (fabsf(w) > 0.00001f) {
        w = 1.0f / w;
    }
    // normalized device coordinates (inverted y)
    Vec3 ndcPos(worldPos.x * w, -worldPos.y * w, worldPos.z * w);
    // 2d screen pos
    return Vec2(
        (ndcPos.x + 1.0f) / 2.0f * ((float)scrWidth),
        (ndcPos.y + 1.0f) / 2.0f * ((float)scrHeight)
    );
}
inline Vec2 WorldToScreenCoords(Vec3& pos, Mat4x4& viewMat, Mat4x4& projMat) {
    Mat4x4 viewProj = projMat * viewMat;
    // we want to calculate and include the w coord
    Vec4 worldPos = viewProj * Vec4(pos, 1.0f);
    float w = worldPos.w;
    if (fabsf(w) > 0.00001f) {
        w = 1.0f / w;
    }
    // normalized device coordinates
    Vec3 ndcPos(worldPos.x * w, worldPos.y * w, worldPos.z * w);
    // 2d screen pos
    return Vec2(ndcPos.x, ndcPos.y);
}

struct Plane
{
    float a,b,c,d;

    Plane() :
        a(0.0f),
        b(0.0f),
        c(0.0f),
        d(0.0f)
    {}

    inline void Normalize() {
        float invMag = 1.0f/sqrtf(a*a + b*b + c*c);
        a *= invMag;
        b *= invMag;
        c *= invMag;
        d *= invMag;
    }

    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxplanedotcoord
    inline float PlaneDotCoord(const Vec3& point) const {
        return a*point.x + b*point.y + c*point.z + d;
    }

    // normal faces away from you if you send verts in counterclockwise order
    // https://keisan.casio.com/exec/system/1223596129
    //inline void Init(const Vec3& p0, const Vec3& p1, const Vec3& p2) {
    inline void Init(const Vec3& p2, const Vec3& p1, const Vec3& p0) {
        Vec3 ab = p1-p0;
        Vec3 ac = p2-p0;
        Vec3 abc = Cross(ab,ac);
        a = abc.x; b = abc.y; c = abc.z;
        d = -(a*p0.x + b*p0.y + c*p0.z);
        Normalize();
    }
    inline bool Inside(const Vec3& point, const float radius) const {
        float fDistance; // calculate our distances to each of the planes
        // find the distance to this plane
        fDistance = PlaneDotCoord(point);
        // if this distance is < -radius, we are outside
        return (fDistance >= -radius);
    }
    inline bool Inside(const Vec3& point) const {
        // Inside the plane is defined as the direction the normal is facing
        float result = PlaneDotCoord(point);
        return (result >= 0.0f);
    }
};

struct Frustum
{
    enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };

    Plane planes[NumPlanes]; // planes of the frustum in camera space
    Vec3 nearClip[4]; // verts of the near clip plane in camera space
    Vec3 farClip[4]; // verts of the far clip plane in camera space

    float fov; // field of view in radians
    float aspect; // aspect ratio (width divided by height)
    float near; // near clipping distance
    float far; // far clipping distance

    Frustum() :
        fov(M_PI/4.0f), // default field of view is 90 degrees
        aspect(1.0f), // default aspect ratio is 1:1
        near(1.0f), // default near plane is 1m away from camera
        far(1000.0f) // default far plane is 1000m away from camera
    {}

    inline bool Inside(const Vec3& point) const {
        for (int i=0; i<NumPlanes; ++i) {
            if (!planes[i].Inside(point)) {
                return false;
            }
        }
        return true;
    }
    inline bool Inside(const Vec3& point, const float radius) const {
        for (int i=0; i<NumPlanes; ++i) {
            if (!planes[i].Inside(point, radius)) {
                return false;
            }
        }
        return true;
    }
    
    inline const Plane& Get(Side side) { return planes[side]; }

    inline void SetFOV(float _fov) { fov = _fov; Init(fov, aspect, near, far); }
    inline void SetAspect(float _aspect) { aspect = _aspect; Init(fov, aspect, near, far); }
    inline void SetNear(float _nearClip) { near = _nearClip; Init(fov, aspect, near, far); }
    inline void SetFar(float _farClip) { far = _farClip; Init(fov, aspect, near, far); }
    inline void Init(const float _fov, const float _aspect, const float _near, const float _far) {
        fov = _fov;
        aspect = _aspect;
        near = _near;
        far = _far;

        double tanFovOver2 = tan(fov/2.0f);
        Vec3 nearRight = (near*tanFovOver2) * aspect * g_Right;
        Vec3 farRight = (far*tanFovOver2) * aspect * g_Right;
        Vec3 nearUp = (near*tanFovOver2) * g_Up;
        Vec3 farUp =  (far*tanFovOver2) * g_Up;

        // points start in the upper right and go around counterclockwise
        nearClip[0] = (near*g_Forward) - nearRight + nearUp;
        nearClip[1] = (near*g_Forward) + nearRight + nearUp;
        nearClip[2] = (near*g_Forward) + nearRight - nearUp;
        nearClip[3] = (near*g_Forward) - nearRight - nearUp;

        farClip[0] = (far*g_Forward) - farRight + farUp;
        farClip[1] = (far*g_Forward) + farRight + farUp;
        farClip[2] = (far*g_Forward) + farRight - farUp;
        farClip[3] = (far*g_Forward) - farRight - farUp;

        // now we have all eight points, time to construct 6 planes.
        // the normal points away from you if you use counterclockwise verts
        Vec3 origin(0.0f,0.0f,0.0f);
        planes[Near].Init(nearClip[2],nearClip[1],nearClip[0]);
        planes[Far].Init(farClip[0],farClip[1],farClip[2]);
        planes[Right].Init(farClip[2],farClip[1],origin);
        planes[Top].Init(farClip[1],farClip[0],origin);
        planes[Left].Init(farClip[0],farClip[3],origin);
        planes[Bottom].Init(farClip[3],farClip[2],origin);
    }

    //TODO?
    //void Render()
};

#endif // GCC4MATH_CPP_H_INCLUDED

