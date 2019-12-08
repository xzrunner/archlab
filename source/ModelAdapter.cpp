#include "cgaview/ModelAdapter.h"

#include <polymesh3/Polytope.h>
#include <cga/Geometry.h>
#include <model/Model.h>
#include <model/BrushModel.h>
#include <model/BrushBuilder.h>
#include <node0/SceneNode.h>
#include <node0/CompMaterial.h>
#include <node3/CompModel.h>
#include <node3/CompModelInst.h>
#include <node3/CompAABB.h>
#include <painting3/MaterialMgr.h>

namespace cgav
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

    auto& caabb = node.AddUniqueComp<n3::CompAABB>();
    caabb.DisableSerialize();
    caabb.SetAABB(model->aabb);

    std::unique_ptr<model::ModelExtend> model_ext = std::make_unique<model::BrushModel>();
    cmodel_inst.GetModel()->SetModelExt(model_ext);
}

void ModelAdapter::UpdateModel(const cga::Geometry& geo, const n0::SceneNode& node)
{
    auto poly = geo.GetPoly();
    if (!poly) {
        return;
    }

    model::BrushModel::Brush brush;

    auto& faces = poly->Faces();

    brush.desc.mesh_begin = 0;
    brush.desc.mesh_end   = 1;
    const size_t face_num = faces.size();
    brush.desc.meshes.push_back({ 0, 0, 0, static_cast<int>(face_num) });

    brush.impl = poly;

    auto brush_model = std::make_unique<model::BrushModel>();
    std::vector<model::BrushModel::Brush> brushes;
    brushes.push_back(brush);
    brush_model->SetBrushes(brushes);

    std::shared_ptr<model::Model> model = nullptr;
    auto& color = geo.GetColor();
    if (color.IsValid())
    {
        std::vector<std::vector<std::vector<sm::vec3>>> colors;
        auto& brushes = brush_model->GetBrushes();
        colors.resize(brushes.size());
        for (size_t i = 0, n = brushes.size(); i < n; ++i) {
            auto& faces = brushes[i].impl->Faces();
            for (auto& face : faces) {
                colors[i].push_back(std::vector<sm::vec3>(face->points.size(), color));
            }
        }

        model = model::BrushBuilder::PolymeshFromBrushPNC(*brush_model, colors);
    }
    else
    {
        model = model::BrushBuilder::PolymeshFromBrushPN(*brush_model);
    }

    auto& cmodel = node.GetSharedComp<n3::CompModel>();
    cmodel.SetModel(model);

    auto& cmodel_inst = node.GetUniqueComp<n3::CompModelInst>();
    cmodel_inst.SetModel(model, 0);

    auto& caabb = node.GetUniqueComp<n3::CompAABB>();
    caabb.SetAABB(model->aabb);
}

}