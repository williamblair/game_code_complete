#include "MovementController.h"
#include "SceneNode.h"

#define MAX(a,b) ((a) >= (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

MovementController::MovementController(
        std::shared_ptr<SceneNode> pObject,
        float initialYaw,
        float initialPitch,
        bool rotateWhenLButtonDown) :
    m_object(pObject)
{
    m_object->VGet()->Transform(&m_matToWorld, &m_matFromWorld);
    
    m_fTargetYaw = m_fYaw = Rad2Deg(-initialYaw);
    m_fTargetPitch = m_fPitch = Rad2Deg(initialPitch);
    
    m_maxSpeed = 30.0f; // 30 meters per second
    m_currentSpeed = 0.0f;
    
    Vec3 pos = m_matToWorld.GetPosition();
    //m_matPosition.BuildTranslation(pos);
    m_matPosition = Translate(pos);
    
    Point ptCursor;
    // TODO
    //GetCursorPos(&ptCursor);
    m_lastMousePos = ptCursor;
    
    memset(m_bKey, 0x00, sizeof(m_bKey));
    m_mouseLButtonDown = false;
    m_bRotateWhenLButtonDown = rotateWhenLButtonDown;
}

void MovementController::SetObject(std::shared_ptr<SceneNode> pNewObject)
{
    m_object = pNewObject;
}

void MovementController::OnUpdate(int const deltaMs)
{
    bool bTranslating = false;
    Vec4 atWorld(0,0,0,0);
    Vec4 rightWorld(0,0,0,0);
    Vec4 upWorld(0,0,0,0);
    
    if (m_bKey['W'] || m_bKey['S']) {
        // In D3D, the "look at" default is always +z
        Vec4 at = g_Forward4;
        if (m_bKey['S']) {
            at *= -1;
        }
        
        // This will give us the "look at" vector
        // in world space - we'll use that to move the camera
        //atWorld = m_matToWorld.Xform(at);
        atWorld = Transform(m_matToWorld, at);
        bTranslating = true;
    }
    
    if (m_bKey['A'] || m_bKey['D']) {
        // In D3D, the "right" default is always +x
        Vec4 right = g_Right4;
        if (m_bKey['A']) {
            right *= -1;
        }
        
        // This will give use the "right" vector
        // in world space - we'll use that toe move the camera
        //rightWorld = m_matToWorld.Xform(right);
        rightWorld = Transform(m_matToWorld, right);
        bTranslating = true;
    }
    
    if (m_bKey[' '] || m_bKey['C'] || m_bKey['X']) {
        // In D3D, the "up" default is always the positive Y axis
        Vec4 up = g_Right4; // BJ TODO - bug?
        if (!m_bKey[' ']) {
            up *= -1;
        }
        
        // unlike strafing, up is always up no matter which way you are looking
        upWorld = up;
        bTranslating = true;
    }
    
    // Handling rotation as a result of mouse position
    {
        // secret formula!
        m_fYaw += (m_fTargetYaw - m_fYaw) * 0.35f;
        m_fTargetPitch = MAX(-90, MIN(90,m_fTargetPitch));
        m_fPitch += (m_fTargetPitch - m_fPitch) * 0.35f;
        
        // Calculate the new rotation matrix from the camera yaw and pitch
        Mat4x4 matRot;
        //matRot.BuildYawPitchRoll(Deg2Rad(-m_fYaw), Deg2Rad(m_fPitch), 0);
        matRot.FromYawPitchRoll(Deg2Rad(-m_fYaw), Deg2Rad(m_fPitch), 0);
        
        // Create the new object-to-world matrix and the new world-to-object matrix
        m_matToWorld = matRot * m_matPosition;
        //m_matFromWorld = m_matToWorld.Inverse();
        m_matFromWorld = Inverse(m_matToWorld);
        m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
    }
    
    if (bTranslating) {
        float elapsedTime = float(deltaMs) / 1000.0f;
        
        Vec4 sum = atWorld + rightWorld + upWorld;
        Vec3 direction(sum.x, sum.y, sum.z);
        //direction.Normalize();
        direction = Normalize(direction);
        
        // Ramp the acceleration by the elapsed time
        float numberOfSeconds = 5.0f;
        m_currentSpeed += m_maxSpeed * ((elapsedTime*elapsedTime)/numberOfSeconds);
        if (m_currentSpeed > m_maxSpeed) {
            m_currentSpeed = m_maxSpeed;
        }
        direction *= m_currentSpeed;
        
        Vec3 pos = m_matPosition.GetPosition() + direction;
        m_matPosition.SetPosition(pos);
        m_matToWorld.SetPosition(pos);
        
        //m_matFromWorld = m_matToWorld.Inverse();
        m_matFromWorld = Inverse(m_matToWorld);
        m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
    } else {
        m_currentSpeed = 0.0f;
    }
}

bool MovementController::VOnPointerMove(const Point& mousePos, const int radius)
{
    // There are two modes supporetd by this controller
    
    if (m_bRotateWhenLButtonDown) {
        // mode 1 - rotate the view only when the left mouse button is down
        // only look around if the left button is down
        if (m_lastMousePos != mousePos && m_mouseLButtonDown) {
            m_fTargetYaw = m_fTargetYaw + (m_lastMousePos.x - mousePos.x);
            m_fTargetPitch = m_fTargetPitch + (mousePos.y - m_lastMousePos.y);
            m_lastMousePos = mousePos;
        }
    } else if (m_lastMousePos != mousePos) {
        // mode 2 - rotate the controller when the mouse buttons are up
        m_fTargetYaw = m_fTargetYaw + (m_lastMousePos.x - mousePos.x);
        m_fTargetPitch = m_fTargetPitch + (mousePos.y - m_lastMousePos.y);
        m_lastMousePos = mousePos;
    }
    return true;
}

bool MovementController::VOnPointerButtonDown(const Point& mousePos, const int radius, const std::string& buttonName)
{
    if (buttonName == "PointerLeft") {
        m_mouseLButtonDown = true;
        
        // we want mouse movement to be relative to the position
        // the cursor was at when the user first presses down on
        // the left button
        m_lastMousePos = mousePos;
        return true;
    }
    return false;
}

bool MovementController::VOnPointerButtonUp(const Point& mousePos, const int radius, const std::string& buttonName)
{
    if (buttonName == "PointerLeft") {
        m_mouseLButtonDown = false;
        return true;
    }
    return false;
}

