#include <cassert>
#include <cstring>
#include "TransformComponent.h"

const char* TransformComponent::g_Name = "TransformComponent.h";

bool TransformComponent::VInit(XMLElement* pData)
{
    assert(pData);
    
    // because changes to the TransformComponents can come in partial
    // definitions, such as from the editor, its better to grab the
    // current values rather than clear them out.
    Vec3 yawPitchRoll = m_Transform.GetYawPitchRoll();
    yawPitchRoll.x = Rad2Deg(yawPitchRoll.x);
    yawPitchRoll.y = Rad2Deg(yawPitchRoll.y);
    yawPitchRoll.z = Rad2Deg(yawPitchRoll.z);
    Vec3 position = m_Transform.GetPosition();
    
    XMLElement* pPosElem = pData->FirstChildElement("Position");
    if (pPosElem)
    {
        double x = 0.0, y = 0.0, z = 0.0;
        x = pPosElem->DoubleAttribute("x");
        y = pPosElem->DoubleAttribute("y");
        z = pPosElem->DoubleAttribute("z");
        position = Vec3(float(x),float(y),float(z));
    }
    
    XMLElement* pOrientElem = pData->FirstChildElement("YawPitchRoll");
    if (pOrientElem)
    {
        double yaw = 0.0, pitch = 0.0, roll = 0.0;
        yaw = pOrientElem->DoubleAttribute("x");
        pitch = pOrientElem->DoubleAttribute("y");
        roll = pOrientElem->DoubleAttribute("z");
        yawPitchRoll = Vec3(float(yaw),float(pitch),float(roll));
    }
    
    Mat4x4 translation;
    translation = Translate(position);
        
    Mat4x4 rotation;
    rotation.FromYawPitchRoll(
        Deg2Rad(yawPitchRoll.x),
        Deg2Rad(yawPitchRoll.y),
        Deg2Rad(yawPitchRoll.z)
    );
    
    m_Transform = translation * rotation;
    
    return true;
}

XMLElement* TransformComponent::VGenerateXml(XMLDocument* pDoc)
{
    char msg[256];
    
    assert(pDoc);
    
    XMLElement* pBaseElem = pDoc->NewElement(VGetName());
    
    // initial transform -> position
    XMLElement* pPos = pBaseElem->InsertNewChildElement("Position");
    Vec3 pos = m_Transform.GetPosition();
    sprintf(msg, "%f", pos.x);
    pPos->SetAttribute("x", msg);
    sprintf(msg, "%f", pos.y);
    pPos->SetAttribute("y", msg);
    sprintf(msg, "%f", pos.z);
    pPos->SetAttribute("z", msg);
    pBaseElem->LinkEndChild(pPos);
    
    // initial transform -> LookAt
    XMLElement* pDir = pBaseElem->InsertNewChildElement("YawPitchRoll");
    Vec3 orient = m_Transform.GetYawPitchRoll();
    orient.x = Rad2Deg(orient.x);
    orient.y = Rad2Deg(orient.y);
    orient.z = Rad2Deg(orient.z);
    sprintf(msg, "%f", orient.x);
    pDir->SetAttribute("x", msg);
    sprintf(msg, "%f", orient.y);
    pDir->SetAttribute("y", msg);
    sprintf(msg, "%f", orient.z);
    pDir->SetAttribute("z", msg);
    pBaseElem->LinkEndChild(pDir);
    
    return pBaseElem;
}

