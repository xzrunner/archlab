#pragma once

#include <wx/panel.h>

namespace ee0 { class WxCodeCtrl; }
namespace cga { class EvalRule; }

namespace cgav
{

class WxTextPage : public wxPanel
{
public:
    WxTextPage(wxWindow* parent);

    std::string GetText() const;
    void SetText(const std::string& text);

    auto GetEval() const { return m_eval; }

private:
    void InitLayout();

    void OnRunPress(wxCommandEvent& event);
    void RebuildEval();

private:
    ee0::WxCodeCtrl* m_code;

    std::shared_ptr<cga::EvalRule> m_eval = nullptr;

}; // WxTextPage

}