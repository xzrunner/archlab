#pragma once

#include <ce/typedef.h>
#include <SM_Vector.h>
#include <model/BrushModel.h>

#include <vector>

namespace ce { class Geometry; }
namespace n0 { class SceneNode; }
namespace model { struct Model; }

namespace cev
{

class ModelAdapter
{
public:
    static void SetupModel(n0::SceneNode& node);

    static void UpdateModel(const std::vector<ce::GeoPtr>& geos,
        const n0::SceneNode& node);

    static bool BuildModel(n0::SceneNode& node);

private:
    static std::unique_ptr<model::Model>
        CreateBrushModel(const std::vector<ce::GeoPtr>& geos);
    static std::unique_ptr<model::Model>
        CreateModelFromFilepath(const std::vector<ce::GeoPtr>& geos);

    static void GeoToBrush(const ce::GeoPtr& geo,
        std::vector<std::vector<std::vector<sm::vec3>>>& colors,
        std::vector<model::BrushModel::Brush>& brushes);

}; // ModelAdapter

}