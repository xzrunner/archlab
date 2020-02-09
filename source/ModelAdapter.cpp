#include "cev/ModelAdapter.h"

#include <ce/Geometry.h>
#include <ce/TopoPolyAdapter.h>
#include <ce/EvalRule.h>
#include <cgaeasy/CompCGA.h>
#include <model/AssimpHelper.h>
#include <model/Model.h>
#include <model/BrushBuilder.h>
#include <node0/SceneNode.h>
#include <node0/CompMaterial.h>
#include <node3/CompModel.h>
#include <node3/CompModelInst.h>
#include <node3/CompAABB.h>
#include <node3/CompShape.h>
#include <halfedge/Polygon.h>
#include <painting3/MaterialMgr.h>
#include <geoshape/Polygon3D.h>
#include <polymesh3/Polytope.h>

namespace cev
{

// todo: copy from sop::GeoAdaptor::Init
void ModelAdapter::SetupModel(n0::SceneNode& node)
{
    auto& cmodel = node.AddSharedComp<n3::CompModel>();
    cmodel.DisableSerialize();
    auto& cmodel_inst = node.AddUniqueComp<n3::CompModelInst>();
    cmodel_inst.DisableSerialize();

    auto& cmaterial = node.AddUniqueComp<n0::CompMaterial>();
    cmaterial.DisableSerialize();
    auto mat = std::make_unique<pt0::Material>();
    typedef pt3::MaterialMgr::PhongUniforms UNIFORMS;
    mat->AddVar(UNIFORMS::ambient.name, pt0::RenderVariant(sm::vec3(0.04f, 0.04f, 0.04f)));
    mat->AddVar(UNIFORMS::diffuse.name, pt0::RenderVariant(sm::vec3(1, 1, 1)));
    mat->AddVar(UNIFORMS::specular.name, pt0::RenderVariant(sm::vec3(1, 1, 1)));
    mat->AddVar(UNIFORMS::shininess.name, pt0::RenderVariant(50.0f));
    cmaterial.SetMaterial(mat);

    auto model = std::make_shared<model::Model>();
    cmodel.SetModel(model);

    cmodel_inst.SetModel(model, 0);

    auto& caabb = node.HasUniqueComp<n3::CompAABB>() ?
        node.GetUniqueComp<n3::CompAABB>() : node.AddUniqueComp<n3::CompAABB>();
    caabb.DisableSerialize();
    caabb.SetAABB(model->aabb);

    std::unique_ptr<model::ModelExtend> model_ext = std::make_unique<model::BrushModel>();
    cmodel_inst.GetModel()->SetModelExt(model_ext);
}

void ModelAdapter::UpdateModel(const std::vector<ce::GeoPtr>& geos, const n0::SceneNode& node)
{
    if (geos.empty()) {
        return;
    }

    std::shared_ptr<model::Model> model = CreateModelFromFilepath(geos);
    if (!model) {
        model = CreateBrushModel(geos);
    }
    assert(model);

    auto& cmodel = node.GetSharedComp<n3::CompModel>();
    cmodel.SetModel(model);

    auto& cmodel_inst = node.GetUniqueComp<n3::CompModelInst>();
    cmodel_inst.SetModel(model, 0);

    auto& caabb = node.GetUniqueComp<n3::CompAABB>();
    caabb.SetAABB(model->aabb);
}

bool ModelAdapter::BuildModel(n0::SceneNode& node)
{
    if (!node.HasUniqueComp<cgae::CompCGA>()) {
        return false;
    }

    auto& ccga = node.GetUniqueComp<cgae::CompCGA>();
    auto rule = ccga.GetRule();
    if (!rule) {
        return false;
    }

    std::vector<ce::GeoPtr> in_geos;
    if (node.HasUniqueComp<n3::CompShape>())
    {
        auto& cshape = node.GetUniqueComp<n3::CompShape>();
        auto& shapes = cshape.GetShapes();
        for (auto& s : shapes)
        {
            if (s->get_type() != rttr::type::get<gs::Polygon3D>()) {
                continue;
            }

            auto& verts = std::static_pointer_cast<gs::Polygon3D>(s)->GetVertices();
            ce::TopoPolyAdapter adapter(verts);
            std::vector<pm3::Polytope::PointPtr> dst_pts;
            std::vector<pm3::Polytope::FacePtr> dst_faces;
            adapter.TransToPolymesh(dst_pts, dst_faces);
            auto poly = std::make_shared<pm3::Polytope>(dst_pts, dst_faces);
            in_geos.push_back(std::make_shared<ce::Geometry>(poly));
        }
    }

    auto out_geos = rule->Eval(in_geos, *ccga.GetRuleCtx());

    if (!out_geos.empty())
    {
        if (!node.HasSharedComp<n3::CompModel>()) {
            SetupModel(node);
        }
        UpdateModel(out_geos, node);
    }

    return true;
}

std::unique_ptr<model::Model>
ModelAdapter::CreateModelFromFilepath(const std::vector<ce::GeoPtr>& geos)
{
    for (auto& geo : geos)
    {
        if (!geo) {
            continue;
        }
        auto& geo_path = geo->GetFilepath();
        if (geo_path.empty()) {
            continue;
        }

        auto model = std::make_unique<model::Model>();
        if (model::AssimpHelper::Load(*model, geo_path)) {
            return model;
        }
    }
    return nullptr;
}

std::unique_ptr<model::Model>
ModelAdapter::CreateBrushModel(const std::vector<ce::GeoPtr>& geos)
{
    assert(!geos.empty());

    std::vector<std::vector<std::vector<sm::vec3>>> colors;

    std::vector<model::BrushModel::Brush> brushes;
    for (auto& geo : geos) {
        GeoToBrush(geo, colors, brushes);
    }

    auto brush_model = std::make_unique<model::BrushModel>();
    brush_model->SetBrushes(brushes);
    return model::BrushBuilder::PolymeshFromBrushPNC(*brush_model, colors);
}

void ModelAdapter::GeoToBrush(const ce::GeoPtr& geo,
                              std::vector<std::vector<std::vector<sm::vec3>>>& colors,
                              std::vector<model::BrushModel::Brush>& brushes)
{
    if (!geo) {
        return;
    }
    auto poly = geo->GetPoly();
    if (!poly)
    {
        assert(!geo->GetChildren().empty());
        for (auto& c : geo->GetChildren()) {
            GeoToBrush(c, colors, brushes);
        }
        return;
    }

    model::BrushModel::Brush brush;

    assert(geo->GetChildren().empty());
    auto& faces = poly->Faces();

    brush.desc.mesh_begin = 0;
    brush.desc.mesh_end = 1;
    const size_t face_num = faces.size();
    brush.desc.meshes.push_back({ 0, 0, 0, static_cast<int>(face_num) });

    brush.impl = poly;

    brushes.push_back(brush);

    std::vector<std::vector<sm::vec3>> b_colors;
    auto color = geo->GetColor();
    if (!color.IsValid()) {
        color.Set(1, 1, 1);
    }
    auto& pts = poly->Points();
    for (auto& face : poly->Faces()) {
        b_colors.push_back(std::vector<sm::vec3>(pts.size(), color));
    }
    colors.push_back(b_colors);
}

}