#include "cgaview/PreviewPage.h"
#include "cgaview/MessageID.h"
#include "cgaview/Serializer.h"
#include "cgaview/ModelAdapter.h"
#include "cgaview/MessageID.h"
#include "cgaview/WxGraphPage.h"
#include "cgaview/Evaluator.h"
#include "cgaview/MessageID.h"

#include <ee0/SubjectMgr.h>
#include <ee0/GameObj.h>
#include <ee0/WxStagePage.h>
#include <ee3/WxStageCanvas.h>
#include <ee3/WorldTravelOP.h>
#include <ee3/CameraDriveOP.h>
#include <draft3/PolygonBuildOP.h>
#include <draft3/PolygonSelectOP.h>

#include <guard/check.h>
#include <node0/SceneNode.h>
#include <node3/CompShape.h>
#include <node3/CompModel.h>
#include <cga/EvalRule.h>
#include <cga/Geometry.h>
#include <cga/TopoPolyAdapter.h>
#include <cgaeasy/CompCGA.h>
#include <geoshape/Polygon3D.h>
#include <halfedge/Polygon.h>
#include <polymesh3/Polytope.h>

namespace cgav
{

PreviewPage::PreviewPage(ee0::WxStagePage& stage_page)
    : m_stage_page(stage_page)
{
    m_messages.push_back(ee0::MSG_SCENE_NODE_INSERT);
    m_messages.push_back(MSG_SET_EDIT_OP);
    m_messages.push_back(MSG_SET_SELECT_OP);
    for (auto& msg : m_messages) {
        stage_page.GetSubjectMgr()->RegisterObserver(msg, this);
    }
}

PreviewPage::~PreviewPage()
{
    for (auto& msg : m_messages) {
        m_stage_page.GetSubjectMgr()->UnregisterObserver(msg, this);
    }

    if (m_graph_page) {
        m_graph_page->GetSubjectMgr()->UnregisterObserver(MSG_RULE_CHANGED, this);
    }
}

void PreviewPage::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    switch (msg)
    {
    case ee0::MSG_SCENE_NODE_INSERT:
    {
        auto var = variants.GetVariant("obj");
        GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
        const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
        GD_ASSERT(obj, "err scene obj");

        auto& ccga = (*obj)->AddUniqueComp<cgae::CompCGA>();
        if (m_graph_page) {
            ccga.SetRule(m_graph_page->GetEval()->GetEval().ToRule());
        }

        BuildModel(**obj);
    }
        break;

    case MSG_SET_EDIT_OP:
        m_stage_page.GetImpl().SetEditOP(m_edit_op);
        break;
    case MSG_SET_SELECT_OP:
        m_stage_page.GetImpl().SetEditOP(m_select_op);
        break;

    case MSG_RULE_CHANGED:
    {
        m_stage_page.GetSubjectMgr()->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

        m_stage_page.Traverse([&](const ee0::GameObj& obj)->bool
        {
            auto& ccga = obj->GetUniqueComp<cgae::CompCGA>();
            ccga.SetRule(m_graph_page->GetEval()->GetEval().ToRule());

            BuildModel(*obj);

            return true;
        });
    }
        break;
    }
}

void PreviewPage::SetGraphPage(WxGraphPage* page)
{
    if (page) {
        page->GetSubjectMgr()->UnregisterObserver(MSG_RULE_CHANGED, this);
    }

    m_graph_page = page;

    page->GetSubjectMgr()->RegisterObserver(MSG_RULE_CHANGED, this);
}

void PreviewPage::InitEditOP()
{
    auto canvas = m_stage_page.GetImpl().GetCanvas();
    assert(canvas);
    auto sub_mgr = m_stage_page.GetSubjectMgr();
    assert(sub_mgr);

    auto& vp = std::static_pointer_cast<ee3::WxStageCanvas>(canvas)->GetViewport();
    m_select_op = std::make_shared<draft3::PolygonSelectOP>(
        canvas->GetCamera(), m_stage_page, vp
    );
    m_edit_op = std::make_shared<draft3::PolygonBuildOP>(
        canvas->GetCamera(), vp, sub_mgr
    );
    m_select_op->SetPrevEditOP(std::make_shared<ee3::WorldTravelOP>(
        canvas->GetCamera(), vp, sub_mgr
    ));
    m_edit_op->SetPrevEditOP(std::make_shared<ee3::CameraDriveOP>(
        canvas->GetCamera(), vp, sub_mgr
    ));
    m_stage_page.GetImpl().SetEditOP(m_edit_op);
}

bool PreviewPage::BuildModel(n0::SceneNode& node)
{
    if (!node.HasUniqueComp<cgae::CompCGA>() ||
        !node.HasUniqueComp<n3::CompShape>()) {
        return false;
    }

    auto& cshape = node.GetUniqueComp<n3::CompShape>();
    auto& shapes = cshape.GetShapes();

    auto& ccga = node.GetUniqueComp<cgae::CompCGA>();
    auto rule = ccga.GetRule();
    if (!rule) {
        return false;
    }

    std::vector<cga::GeoPtr> in_geos;
    for (auto& s : shapes)
    {
        if (s->get_type() != rttr::type::get<gs::Polygon3D>()) {
            continue;
        }

        auto& verts = std::static_pointer_cast<gs::Polygon3D>(s)->GetVertices();
        cga::TopoPolyAdapter adapter(verts);
        std::vector<pm3::Polytope::PointPtr> dst_pts;
        std::vector<pm3::Polytope::FacePtr> dst_faces;
        adapter.TransToPolymesh(dst_pts, dst_faces);
        auto poly = std::make_shared<pm3::Polytope>(dst_pts, dst_faces);
        in_geos.push_back(std::make_shared<cga::Geometry>(poly));
    }
    if (in_geos.empty()) {
        return false;
    }
    auto out_geos = rule->Eval(in_geos);

    if (!out_geos.empty())
    {
        if (!node.HasSharedComp<n3::CompModel>()) {
            ModelAdapter::SetupModel(node);
        }
        ModelAdapter::UpdateModel(*out_geos[0], node);
    }

    return true;
}

}