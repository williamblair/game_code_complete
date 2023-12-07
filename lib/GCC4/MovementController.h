#ifndef GCC4_MOVEMENT_CONTROLLER_H_INCLUDED
#define GCC4_MOVEMENT_CONTROLLER_H_INCLUDED

#include <memory>

#include "GCCMath.h"
#include "IPointerHandler.h"
#include "IKeyboardHandler.h"

// forward declaration
class SceneNode;

class MovementController : public IPointerHandler, public IKeyboardHandler
{
public:

    MovementController(
        std::shared_ptr<SceneNode> pObject,
        float initialYaw,
        float initialPitch,
        bool rotateWhenLButtonDown
    );
    void SetObject(std::shared_ptr<SceneNode> pNewObject);
    
    void OnUpdate(int const deltaMs);

    // IPointerHandler interface
    bool VOnPointerMove(const Point& mousePos, const int radius);
    bool VOnPointerButtonDown(const Point& mousePos, const int radius, const std::string& buttonName);
    bool VOnPointerButtonUp(const Point& mousePos, const int radius, const std::string& buttonName);

    // IKeyboardHandler interface
    bool VOnKeyDown(const char c) { m_bKey[c] = true; return true; }
    bool VOnKeyUp(const char c) { m_bKey[c] = false; return true; }
    
    const Mat4x4* GetToWorld() { return &m_matToWorld; }
    const Mat4x4* GetFromWorld() { return &m_matFromWorld; }
    
protected:
    Mat4x4 m_matFromWorld;
    Mat4x4 m_matToWorld;
    Mat4x4 m_matPosition;
    
    Point m_lastMousePos;
    bool m_bKey[256]; // which keys are up and down
    
    // orientation controls
    float m_fTargetYaw;
    float m_fTargetPitch;
    float m_fYaw;
    float m_fPitch;
    float m_fPitchOnDown;
    float m_fYawOnDown;
    float m_maxSpeed;
    float m_currentSpeed;
    
    bool m_mouseLButtonDown;
    bool m_bRotateWhenLButtonDown;
    
    std::shared_ptr<SceneNode> m_object;
};

#endif // GCC4_MOVEMENT_CONTROLLER_H_INCLUDED
