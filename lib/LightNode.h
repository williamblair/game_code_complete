#ifndef LIGHT_NODE_H_INCLUDED
#define LIGHT_NODE_H_INCLUDED

#include "SceneNode.h"

class LightNode : public SceneNode
{
public:
    
    struct LightProperties
    {
        float m_Attenuation[3]; // attenuation coeffictients
        float m_Range;
        float m_Falloff;
        float m_Theta;
        float m_Phi;

        LightProperties& operator=(const LightProperties& other) {
            m_Attenuation[0] = other.m_Attenuation[0];
            m_Attenuation[1] = other.m_Attenuation[1];
            m_Attenuation[2] = other.m_Attenuation[2];
            m_Range = other.m_Range;
            m_Falloff = other.m_Falloff;
            m_Theta = other.m_Theta;
            m_Phi = other.m_Phi;
            return *this;
        }
    };
    
    LightNode(
        const ActorId actorId,
        std::string name,
        const LightProperties& props,
        const Color& diffuseColor,
        const Mat4x4* t
    );
    
protected:
    
    LightProperties m_LightProps;
};

#endif // LIGHT_NODE_H_INCLUDED
