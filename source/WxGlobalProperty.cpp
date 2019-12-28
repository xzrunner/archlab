#include "cgaview/WxGlobalProperty.h"

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

namespace
{

class WxAddParmDlg : public wxDialog
{
public:
	WxAddParmDlg(wxWindow* parent, const wxPoint& pos = wxDefaultPosition)
		: wxDialog(parent, wxID_ANY, "Create custom property", pos)
	{
		InitLayout();
	}

	int GetSelectIdx() const { return m_type->GetSelection(); }

	std::string GetKey() const { return m_key->GetValue().ToStdString(); }

private:
	void InitLayout()
	{
		wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

		top_sizer->AddSpacer(10);

		// type
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(new wxStaticText(this, wxID_ANY, "Type:"));

			sizer->AddSpacer(80);

            wxArrayString labels;
            labels.push_back("float");
            labels.push_back("string");

			sizer->Add(m_type = new wxChoice(this, wxID_ANY, wxDefaultPosition,
				wxSize(200, -1), labels));
			m_type->SetSelection(0);

			top_sizer->Add(sizer);
		}

		top_sizer->AddSpacer(10);

		// key
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, "Variable name:"));
			sizer->AddSpacer(25);
			sizer->Add(m_key = new wxTextCtrl(this, wxID_ANY,
				wxEmptyString, wxDefaultPosition, wxSize(200, -1)));
			top_sizer->Add(sizer);
		}

		top_sizer->AddSpacer(20);

		// btn
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
			sizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
			top_sizer->Add(sizer, 0, wxALIGN_CENTER);
		}

		SetSizer(top_sizer);
		top_sizer->Fit(this);
	}

private:
	wxChoice*   m_type;
	wxTextCtrl* m_key;

}; // WxAddParmDlg

}

namespace cgav
{

WxGlobalProperty::WxGlobalProperty(wxWindow* parent, cga::EvalContext& ctx)
    : wxPanel(parent)
    , m_ctx(ctx)
{
    InitLayout();
}

void WxGlobalProperty::InitLayout()
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    InitHeader(sizer);
    InitProperty(sizer);
    SetSizer(sizer);
}

void WxGlobalProperty::InitHeader(wxSizer* top_sizer)
{
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    m_add_btn = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
    Connect(m_add_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(WxGlobalProperty::OnAddPress));
    sizer->Add(m_add_btn, 0, wxLEFT | wxRIGHT, 5);

    m_del_btn = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
    Connect(m_del_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(WxGlobalProperty::OnDelPress));
    sizer->Add(m_del_btn, 0, wxLEFT | wxRIGHT, 5);

    top_sizer->Add(sizer);
}

void WxGlobalProperty::InitProperty(wxSizer* top_sizer)
{
    m_pg = new wxPropertyGrid(this, -1, wxDefaultPosition, wxSize(500, -1),
        wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED);
    Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(
        WxGlobalProperty::OnPropertyGridChange));
    top_sizer->Add(m_pg, wxEXPAND);

    ReloadParmProps();
}

void WxGlobalProperty::OnAddPress(wxCommandEvent& event)
{
    WxAddParmDlg dlg(this, m_add_btn->GetScreenPosition());
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }

    cga::EvalContext::Parm p;
    p.name = dlg.GetKey();
    switch (dlg.GetSelectIdx())
    {
    case 0:
        p.val = dag::Variable(0.0f);
        break;
    case 1:
        p.val = dag::Variable(std::string());
        break;
    default:
        assert(0);
    }
    AddParmToProp(p);

    m_ctx.AddGlobalParm(p);
}

void WxGlobalProperty::OnDelPress(wxCommandEvent& event)
{
    auto prop = m_pg->GetSelectedProperty();
    auto name = prop->GetName().ToStdString();
    m_pg->DeleteProperty(prop);
    m_ctx.RemoveGlobalParm(name);
}

void WxGlobalProperty::OnPropertyGridChange(wxPropertyGridEvent& event)
{
}

void WxGlobalProperty::ReloadParmProps()
{
    m_pg->Clear();

    for (auto& p : m_ctx.GetGlobalParms()) {
        AddParmToProp(p);
    }
}

void WxGlobalProperty::AddParmToProp(const cga::EvalContext::Parm& p)
{
    switch (p.val.type)
    {
    case dag::VarType::Float:
        m_pg->Append(new wxFloatProperty(p.name, wxPG_LABEL, p.val.f));
        break;
    case dag::VarType::String:
        m_pg->Append(new wxStringProperty(p.name, wxPG_LABEL, static_cast<const char*>(p.val.p)));
        break;
    default:
        assert(0);
    }
}

}