#include "cgaview/RenderSystem.h"
#include "cgaview/Evaluator.h"

#include <painting3/RenderSystem.h>
#include <node3/RenderSystem.h>

namespace cgav
{

RenderSystem::RenderSystem(const pt3::Viewport& vp,
                           const sm::mat4& cam_mat)
    : m_vp(vp)
    , m_cam_mat(cam_mat)
{
}

void RenderSystem::DrawNode3D(const pt0::RenderContext& rc, const n0::SceneNode& node) const
{
    pt3::RenderParams rp;
    rp.painter  = &m_pt;
    rp.viewport = &m_vp;
    rp.cam_mat  = &m_cam_mat;

    // draw face
    rp.type = pt3::RenderParams::DRAW_MESH;
    n3::RenderSystem::Draw(node, rp, rc);

    // draw edge
    rp.type = pt3::RenderParams::DRAW_BORDER_MESH;
    n3::RenderSystem::Draw(node, rp, rc);
}

}