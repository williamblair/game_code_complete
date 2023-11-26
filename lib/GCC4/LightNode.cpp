#include "LightNode.h"

LightNode::LightNode(
    const ActorId actorId,
    std::string name,
    const LightProperties& props,
    const Color& diffuseColor,
    const Mat4x4* t) :
        SceneNode(actorId, name, RenderPass_NotRendered, diffuseColor, t)
{
    m_LightProps = props;
}

