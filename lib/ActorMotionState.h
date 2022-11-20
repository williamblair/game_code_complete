#ifndef GCC4_ACTOR_MOTION_STATE_H_INCLUDED
#define GCC4_ACTOR_MOTION_STATE_H_INCLUDED

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "GCCMath.h"
#include "Mat4ToBtTransform.h"

struct ActorMotionState : public btMotionState
{
    Mat4x4 m_worldToPositionTransform;
    
    ActorMotionState(Mat4x4 const& startingTransform) :
        m_worldToPositionTransform(startingTransform)
    {}
    
    // btMotionState interface: Bullet calls these
    virtual void getWorldTransform(btTransform& worldTrans) const
    {
        worldTrans = Mat4x4ToBtTransform(m_worldToPositionTransform);
    }
    virtual void setWorldTransform(const btTransform& worldTrans)
    {
        m_worldToPositionTransform = BtTransformToMat4x4(worldTrans);
    }
};

#endif // GCC4_ACTOR_MOTION_STATE_H_INCLUDED
