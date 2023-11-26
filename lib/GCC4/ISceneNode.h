#ifndef GCC4_ISCENENODE_H_INCLUDED
#define GCC4_ISCENENODE_H_INCLUDED

#include <memory>
#include <cstdint>

#include "GCCMath.h"
#include "ComponentTypes.h"

// forward declaration
class SceneNodeProperties;
class Scene;

/**
 * Scene Node Interface
 */
class ISceneNode
{
public:

    virtual SceneNodeProperties* VGet() = 0;
    virtual void VSetTransform(
        const Mat4x4* toWorld,
        const Mat4x4* fromWorld = nullptr
    ) = 0;
    virtual bool VOnUpdate(Scene* pScene, uint32_t const elapsedMs) = 0;
    virtual bool VOnRestore(Scene* pScene) = 0;
    
    virtual bool VPreRender(Scene* pScene) = 0;
    virtual bool VIsVisible(Scene* pScene) = 0;
    virtual bool VRender(Scene* pScene) = 0;
    virtual bool VRenderChildren(Scene* pScene) = 0;
    virtual bool VPostRender(Scene* pScene) = 0;
    
    virtual bool VAddChild(std::shared_ptr<ISceneNode> kid) = 0;
    virtual bool VRemoveChild(ActorId id) = 0;
    
    virtual bool VOnLostDevice(Scene* pScene) = 0;
    
    virtual ~ISceneNode() {}
};

#endif // GCC4_ISCENENODE_H_INCLUDED
