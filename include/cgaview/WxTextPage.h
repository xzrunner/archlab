#pragma once

#include <ee0/typedef.h>

#include <wx/panel.h>

namespace ee0 { class WxCodeCtrl; }
namespace cga { class EvalRule; }

namespace cgav
{

class WxTextPage : public wxPanel
{
public:
    WxTextPage(wxWindow* parent, const ee0::SubjectMgrPtr& preview_sub_mgr);

    std::string GetText() const;
    void SetText(const std::string& text);

    auto GetEval() const { return m_eval; }

private:
    void InitLayout();

    void OnRunPress(wxCommandEvent& event);
    void RebuildEval();

private:
    ee0::SubjectMgrPtr m_preview_sub_mgr = nullptr;

    ee0::WxCodeCtrl* m_code;

    std::shared_ptr<cga::EvalRule> m_eval = nullptr;

}; // WxTextPage

}