#pragma once

#include <ee3/WxStageCanvas.h>

namespace cgav
{

class WxStageCanvas : public ee3::WxStageCanvas
{
public:
    WxStageCanvas(ee0::WxStagePage* stage, ECS_WORLD_PARAM
        const ee0::RenderContext& rc);

    void SetGraphStage(ee0::WxStagePage* stage) { m_graph_stage = stage; }

protected:
    virtual void DrawBackground3D() const override;
    virtual void DrawForeground3D() const override;
    virtual void DrawForeground2D() const override;

private:
    ee0::WxStagePage* m_graph_stage = nullptr;

}; // WxStageCanvas

}