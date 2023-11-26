#include "SceneNodeProperties.h"

void SceneNodeProperties::Transform(
        Mat4x4* toWorld, 
        Mat4x4* fromWorld) const
{
    if (toWorld) {
        *toWorld = m_ToWorld;
    }
    
    if (fromWorld) {
        *fromWorld = m_FromWorld;
    }
}
