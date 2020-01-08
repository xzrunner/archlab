#pragma once

#include <cga/typedef.h>

#include <vector>

namespace cga { class Geometry; }
namespace n0 { class SceneNode; }

namespace cgav
{

class ModelAdapter
{
public:
    static void SetupModel(n0::SceneNode& node);

    static void UpdateModel(const std::vector<cga::GeoPtr>& geos,
        const n0::SceneNode& node);

    static bool BuildModel(n0::SceneNode& node);

}; // ModelAdapter

}