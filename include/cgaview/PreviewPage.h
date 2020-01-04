#pragma once

#include <ee0/typedef.h>
#include <ee0/Observer.h>

#include <vector>

namespace ee0 { class WxStagePage; }
namespace n0 { class SceneNode; }

namespace cgav
{

class WxGraphPage;

class PreviewPage : public ee0::Observer
{
public:
    PreviewPage(ee0::WxStagePage& stage_page);
    virtual ~PreviewPage();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    void SetGraphPage(WxGraphPage* page);

    void InitEditOP();

private:
    static bool BuildModel(n0::SceneNode& node);

private:
    ee0::WxStagePage& m_stage_page;

    std::vector<uint32_t> m_messages;

    WxGraphPage* m_graph_page = nullptr;

    ee0::EditOPPtr m_view_op = nullptr;
    ee0::EditOPPtr m_edit_op = nullptr;

}; // PreviewPage

}