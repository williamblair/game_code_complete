#include <cstdio>
#include "BulletDebugDrawer.h"

void BulletDebugDrawer::drawLine(
    const btVector3& from,
    const btVector3& to,
    const btVector3& color)
{
    //TODO
}
void BulletDebugDrawer::drawContactPoint(
    const btVector3& PointOnB,
    const btVector3& NormalOnB,
    btScalar distance,
    int lifeTime,
    const btVector3& color)
{
    //TODO
}
void BulletDebugDrawer::reportErrorWarning(const char* warningString)
{
    printf("BulletDebudDrawere reportErrorWarning: %s\n", warningString);
}
void BulletDebugDrawer::draw3dText(
    const btVector3& location,
    const char* textString)
{
    //TODO
}
void BulletDebugDrawer::setDebugMode(int debugMode)
{
    //TODO
}
int BulletDebugDrawer::getDebugMode() const
{
    //TODO
    return 0;
}


