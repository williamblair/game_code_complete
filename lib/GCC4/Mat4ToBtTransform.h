#ifndef GCC4_MAT4_TO_BTTRANSFORM_H_INCLUDED
#define GCC4_MAT4_TO_BTTRANSFORM_H_INCLUDED

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "GCCMath.h"

static inline btTransform Mat4x4ToBtTransform(Mat4x4 const& mat)
{
    btMatrix3x3 bulletRotation;
    btVector3 bulletPosition;
    
    // copy rotation matrix
    for (int row=0; row<3; ++row) {
        for (int col=0; col<3; ++col) {
            bulletRotation[row][col] = mat(row,col);
        }
    }
    
    // copy position
    Vec3 matPos = mat.GetPosition();
    //for (int col=0; col<3; ++col) {
    //    bulletPosition[col] = matPos.v[col];
    //}
    bulletPosition[0] = matPos.x;
    bulletPosition[1] = matPos.y;
    bulletPosition[2] = matPos.z;
    
    return btTransform(bulletRotation, bulletPosition);
}

static inline Mat4x4 BtTransformToMat4x4(btTransform const& trans)
{
    Mat4x4 retVal = Mat4x4::g_Identity;
    
    btMatrix3x3 const& bulletRotation = trans.getBasis();
    btVector3 const& bulletPosition = trans.getOrigin();
    
    // copy rotation matrix
    for (int row=0; row<3; ++row) {
        for (int col=0; col<3; ++col) {
            retVal.v[col][row] = bulletRotation[row][col];
        }
    }
    
    // copy position
    retVal.r0c3 = bulletPosition[0];
    retVal.r1c3 = bulletPosition[1];
    retVal.r2c3 = bulletPosition[2];
    
    return retVal;
}

#endif // GCC4_MAT4_TO_BTTRANSFORM_H_INCLUDED
