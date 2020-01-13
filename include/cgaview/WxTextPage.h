#pragma once

#include <ee0/typedef.h>

#include <wx/panel.h>

namespace ee0 { class WxCodeCtrl; }
namespace cga { class EvalRule; }

namespace cgav
{

class Scene;

class WxTextPage : public wxPanel
{
public:
    WxTextPage(wxWindow* parent, Scene& scene,
        const ee0::SubjectMgrPtr& preview_sub_mgr);

    std::string GetText() const;
    void SetText(const std::string& text);

    auto GetEval() const { return m_eval; }

    void SetRulePath(const std::string& path) { m_rule_path = path; }

    static constexpr char* const FILEPATH = "default_text_rule";

private:
    void InitLayout();

    void OnRunPress(wxCommandEvent& event);
    void RebuildEval();

private:
    Scene& m_scene;

    ee0::SubjectMgrPtr m_preview_sub_mgr = nullptr;

    ee0::WxCodeCtrl* m_code;

    std::shared_ptr<cga::EvalRule> m_eval = nullptr;

    std::string m_rule_path;

}; // WxTextPage

}