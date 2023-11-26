#ifndef GCC4_ROOT_NODE_H_INCLUDED
#define GCC4_ROOT_NODE_H_INCLUDED

#include <memory>

#include "SceneNode.h"
#include "RenderPass.h"

class RootNode : public SceneNode
{
public:
    RootNode();
    virtual bool VAddChild(std::shared_ptr<ISceneNode> kid);
    virtual bool VRenderChildren(Scene* pScene);
    virtual bool VIsVisible(Scene* pScene) { return true; }
};

#endif // GCC4_ROOT_NODE_H_INCLUDED
