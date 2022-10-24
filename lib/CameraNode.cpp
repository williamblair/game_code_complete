#include "CameraNode.h"

CameraNode::CameraNode(Mat4x4 const* t, Frustum const& frustum) :
    SceneNode(INVALID_ACTOR_ID, "Camera", RenderPass_0, g_Black, t),
    m_Frustum(frustum),
    m_bActive(true),
    m_DebugCamera(false),
    m_pTarget(std::shared_ptr<SceneNode>()),
    m_CamOffsetVector(0.0f,1.0f,-10.0f,0.0f)
{
}

bool CameraNode::VRender(Scene* pScene)
{
    //TODO
    //if (m_DebugCamera) { m_Frustum.Render(); }
    return true;
}

bool CameraNode::VOnRestore(Scene* pScene)
{
    //TODO
    //m_Frustum.SetAspect(DXUTGetWindowWidth()) / (float)DXUTGetWindowHeight());
    m_Projection = Perspective(m_Frustum.fov, m_Frustum.aspect, m_Frustum.near, m_Frustum.far);
}

bool CameraNode::SetView(Scene* pScene)
{
    // If there is a target, make sure the camera is
    // rigidly attaced right behind the target
    if (m_pTarget)
    {
        Mat4x4 mat = m_pTarget->VGet()->GetToWorld();
        Vec4 at = m_CamOffsetVector;
        Vec4 atWorld = Transform(mat, at);
        Vec3 pos = mat.GetPosition() + Vec3(atWorld.x,atWorld.y,atWorld.z);
        mat.SetPosition(pos);
        VSetTransform(&mat);
    }
    return true;
}

