#include "GCCMath.h"

Vec3 g_Forward(0.0f,0.0f,-1.0f);
Vec3 g_Right(1.0f,0.0f,0.0f);
Vec3 g_Up(0.0f,1.0f,0.0f);

Vec4 g_Forward4(0.0f,0.0f,-1.0f,0.0f);
Vec4 g_Right4(1.0f,0.0f,0.0f,0.0f);
Vec4 g_Up4(0.0f,1.0f,0.0f,0.0f);

// static member instance
Mat4x4 Mat4x4::g_Identity;
