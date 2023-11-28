#ifndef GCC4_BULLET_DEBUG_DRAWER_H_INCLUDED
#define GCC4_BULLET_DEBUG_DRAWER_H_INCLUDED

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

class BulletDebugDrawer : public btIDebugDraw
{
public:
    // btIDebugDraw interface
    virtual void drawLine(
        const btVector3& from,
        const btVector3& to,
        const btVector3& color
    );
    virtual void drawContactPoint(
        const btVector3& PointOnB,
        const btVector3& NormalOnB,
        btScalar distance,
        int lifeTime,
        const btVector3& color
    );
    virtual void reportErrorWarning(const char* warningString);
    virtual void draw3dText(
        const btVector3& location,
        const char* textString
    );
    virtual void setDebugMode(int debugMode);
    virtual int getDebugMode() const;
};

#endif // GCC4_BULLET_DEBUG_DRAWER_H_INCLUDED

