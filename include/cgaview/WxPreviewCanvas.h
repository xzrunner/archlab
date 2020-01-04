#pragma once

#include <ee3/WxStageCanvas.h>

namespace cgav
{

class WxPreviewCanvas : public ee3::WxStageCanvas
{
public:
    WxPreviewCanvas(ee0::WxStagePage* stage, ECS_WORLD_PARAM
        const ee0::RenderContext& rc);
    virtual ~WxPreviewCanvas();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

protected:
    virtual void DrawBackground3D() const override;
    virtual void DrawForeground3D() const override;
    virtual void DrawForeground2D() const override;

private:
    enum class OpType
    {
        Edit,
        Select,
    };

private:
    OpType m_op_type = OpType::Edit;

}; // WxPreviewCanvas

}