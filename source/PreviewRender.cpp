#include "cgaview/PreviewRender.h"
#include "cgaview/Evaluator.h"

#include <painting3/RenderSystem.h>
#include <node0/SceneNode.h>
#include <node3/RenderSystem.h>
#include <node3/CompShape.h>

namespace cgav
{

PreviewRender::PreviewRender(const pt3::Viewport& vp,
                           const sm::mat4& cam_mat)
    : m_vp(vp)
    , m_cam_mat(cam_mat)
{
}

void PreviewRender::DrawNode3D(const pt0::RenderContext& rc, const n0::SceneNode& node) const
{
    pt3::RenderParams rp;
    rp.painter  = &m_pt;
    rp.viewport = &m_vp;
    rp.cam_mat  = &m_cam_mat;

    // draw shapes
    if (node.HasSharedComp<n3::CompShape>())
    {
        auto& cshape = node.GetSharedComp<n3::CompShape>();
        auto& shapes = cshape.GetShapes();
        if (!shapes.empty()) {
            for (auto& s : shapes) {
                pt3::RenderSystem::DrawShape(*s, rp);
            }
        }
    }

    // draw face
    rp.type = pt3::RenderParams::DRAW_MESH;
    n3::RenderSystem::Draw(node, rp, rc);

    // draw edge
    rp.type = pt3::RenderParams::DRAW_BORDER_MESH;
    n3::RenderSystem::Draw(node, rp, rc);
}

}