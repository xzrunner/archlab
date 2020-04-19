#pragma once

#include <ee0/typedef.h>

#include <node0/typedef.h>

#include <wx/panel.h>

namespace ee0 { class WxCodeCtrl; }
namespace archgraph { class EvalContext; class EvalRule; }
namespace cga { class StringPool; }
namespace ur2 { class Device; }

namespace archlab
{

class Scene;

class WxTextPage : public wxPanel
{
public:
    WxTextPage(const ur2::Device& dev, wxWindow* parent, Scene& scene,
        const ee0::SubjectMgrPtr& preview_sub_mgr);

    std::string GetText() const;
    void SetText(const std::string& text);

    auto GetEval() const { return m_eval; }

    void SetRulePath(const std::string& path) { m_rule_path = path; }
    auto& GetRulePath() const { return m_rule_path; }

    auto GetPreviewObj() const { return m_preview_obj; }

    auto GetStringPool() const { return m_str_pool; }

private:
    void InitLayout();

    void OnRunPress(wxCommandEvent& event);
    void RebuildEval();

private:
    std::shared_ptr<cga::StringPool> m_str_pool = nullptr;

    Scene& m_scene;

    ee0::SubjectMgrPtr m_preview_sub_mgr = nullptr;

    ee0::WxCodeCtrl* m_code;

    std::shared_ptr<archgraph::EvalContext> m_eval_ctx = nullptr;
    std::shared_ptr<archgraph::EvalRule>    m_eval     = nullptr;

    std::string m_rule_path = "default_text_rule";

    n0::SceneNodePtr m_preview_obj = nullptr;

}; // WxTextPage

}