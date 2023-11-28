#ifndef GCC4_CAMERA_NODE_H_INCLUDED
#define GCC4_CAMERA_NODE_H_INCLUDED

#include <memory>

#include "GCCMath.h"
#include "SceneNode.h"

class CameraNode : public SceneNode
{
public:
    CameraNode(Mat4x4 const* t, Frustum const& frustum);
    virtual bool VRender(Scene* pScene);
    virtual bool VOnRestore(Scene* pScene);
    virtual bool VIsVisible(Scene* pScene) const { return m_bActive; }
    virtual bool SetViewTransform(Scene* pScene);
    const Frustum& GetFrustum() { return m_Frustum; }
    void SetTarget(std::shared_ptr<SceneNode> pTarget) { m_pTarget = pTarget; }
    void ClearTarget() { m_pTarget = std::shared_ptr<SceneNode>(); }

    Mat4x4 GetWorldViewProjection(Scene* pScene);

    Mat4x4 GetProjection() { return m_Projection; }
    Mat4x4 GetView() { return m_View; }

    void SetCameraOffset(const Vec4& cameraOffset) { m_CamOffsetVector = cameraOffset; }

protected:
    Frustum m_Frustum;
    Mat4x4 m_Projection;
    Mat4x4 m_View;
    bool m_bActive;
    bool m_DebugCamera;
    std::shared_ptr<SceneNode> m_pTarget;
    Vec4 m_CamOffsetVector;
};

#endif // GCC4_CAMERA_NODE_H_INCLUDED

