#pragma once

#include <cga/EvalContext.h>

#include <wx/panel.h>

class wxPropertyGrid;
class wxPropertyGridEvent;

namespace cgav
{

class WxGlobalProperty : public wxPanel
{
public:
    WxGlobalProperty(wxWindow* parent, cga::EvalContext& ctx);

private:
    void InitLayout();

    void InitHeader(wxSizer* sizer);
    void InitProperty(wxSizer* sizer);

    void OnAddPress(wxCommandEvent& event);
    void OnDelPress(wxCommandEvent& event);

    void OnPropertyGridChange(wxPropertyGridEvent& event);

    void ReloadParmProps();
    void AddParmToProp(const cga::EvalContext::Parm& parm);

private:
    wxPropertyGrid* m_pg;

    wxButton* m_add_btn;
    wxButton* m_del_btn;

    cga::EvalContext& m_ctx;

}; // WxGlobalProperty

}