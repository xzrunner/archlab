#pragma once

#include <archgraph/EvalContext.h>

#include <wx/panel.h>

class wxPropertyGrid;
class wxPropertyGridEvent;

namespace archlab
{

class WxRuleProperty : public wxPanel
{
public:
    WxRuleProperty(wxWindow* parent, archgraph::EvalContext& ctx);

private:
    void InitLayout();

    void InitHeader(wxSizer* sizer);
    void InitProperty(wxSizer* sizer);

    void OnAddPress(wxCommandEvent& event);
    void OnDelPress(wxCommandEvent& event);

    void OnPropertyGridChange(wxPropertyGridEvent& event);

    void ReloadParmProps();
    void AddParmToProp(const archgraph::EvalContext::Parm& parm);

private:
    wxPropertyGrid* m_pg;

    wxButton* m_add_btn;
    wxButton* m_del_btn;

    archgraph::EvalContext& m_ctx;

}; // WxRuleProperty

}