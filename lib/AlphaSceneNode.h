#ifndef ALPHA_SCENE_NODE_H_INCLUDED
#define ALPHA_SCENE_NODE_H_INCLUDED

#include <memory>
#include <list>

#include <ISceneNode.h>
#include <GCCMath.h>

struct AlphaSceneNode
{
    std::shared_ptr<ISceneNode> m_pNode;
    Mat4x4 m_Concat;
    float m_ScreenZ;

    // for STL sort
    bool const operator<(AlphaSceneNode const& other) {
        //return m_ScreenZ < other.m_ScreenZ;
        // values with larger z are closer to the screen (b/c right handed coordinates)
        return m_ScreenZ > other.m_ScreenZ;
    }
};

typedef std::list<AlphaSceneNode*> AlphaSceneNodes;

#endif // ALPHA_SCENE_NODE_H_INCLUDED
