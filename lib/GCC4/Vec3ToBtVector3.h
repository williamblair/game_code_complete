#ifndef GCC4_VEC3_TO_BTVECTOR3_H_INCLUDED
#define GCC4_VEC3_TO_BTVECTOR3_H_INCLUDED

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "GCCMath.h"

static inline btVector3 Vec3ToBtVector3(Vec3 const& vec3)
{
    return btVector3(vec3.x,vec3.y,vec3.z);
}
static inline Vec3 BtVector3ToVec3(btVector3 const& btVec)
{
    return Vec3(btVec.x(),btVec.y(),btVec.z());
}

#endif // GCC4_VEC3_TO_BTVECTOR3_H_INCLUDED
