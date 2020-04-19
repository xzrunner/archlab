#pragma once

#include <archgraph/typedef.h>
#include <SM_Vector.h>
#include <model/BrushModel.h>

#include <vector>

namespace archgraph { class Geometry; }
namespace n0 { class SceneNode; }
namespace model { struct Model; }
namespace ur2 { class Device; }

namespace archlab
{

class ModelAdapter
{
public:
    static void SetupModel(const ur2::Device& dev, n0::SceneNode& node);

    static void UpdateModel(const ur2::Device& dev,
        const std::vector<archgraph::GeoPtr>& geos, const n0::SceneNode& node);

    static bool BuildModel(const ur2::Device& dev, n0::SceneNode& node);

private:
    static std::unique_ptr<model::Model> CreateBrushModel(const ur2::Device& dev,
        const std::vector<archgraph::GeoPtr>& geos);
    static std::unique_ptr<model::Model> CreateModelFromFilepath(const ur2::Device& dev,
            const std::vector<archgraph::GeoPtr>& geos);

    static void GeoToBrush(const archgraph::GeoPtr& geo,
        std::vector<std::vector<std::vector<sm::vec3>>>& colors,
        std::vector<model::BrushModel::Brush>& brushes);

}; // ModelAdapter

}