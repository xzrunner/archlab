#pragma once

#include <ee0/typedef.h>
#include <ee0/Observer.h>

#include <node0/typedef.h>

#include <vector>

namespace ee0 { class WxStagePage; }
namespace n0 { class SceneNode; }

namespace cgav
{

class Scene;

class PreviewPage : public ee0::Observer
{
public:
    PreviewPage(ee0::WxStagePage& stage_page);
    virtual ~PreviewPage();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    void InitEditOP();

    void InitSceneNodeRule(const Scene& scene);

    auto GetGraphObj() const { return m_graph_obj; }
    auto GetTextObj() const { return m_text_obj; }

private:
    ee0::WxStagePage& m_stage_page;

    std::vector<uint32_t> m_messages;

    ee0::EditOPPtr m_select_op = nullptr;
    ee0::EditOPPtr m_edit_op = nullptr;

    n0::SceneNodePtr m_graph_obj = nullptr;
    n0::SceneNodePtr m_text_obj  = nullptr;

}; // PreviewPage

}