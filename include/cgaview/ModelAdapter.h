#pragma once

namespace cga { class Geometry; }
namespace n0 { class SceneNode; }

namespace cgav
{

class ModelAdapter
{
public:
    static void SetupModel(n0::SceneNode& node);

    static void UpdateModel(const cga::Geometry& geo,
        const n0::SceneNode& node);

    static bool BuildModel(n0::SceneNode& node);

}; // ModelAdapter

}