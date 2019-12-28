#include "cgaview/WxTextPage.h"

#include <ee0/WxCodeCtrl.h>

#include <wx/sizer.h>
#include <wx/button.h>

namespace cgav
{

WxTextPage::WxTextPage(wxWindow* parent)
    : wxPanel(parent)
{
    InitLayout();
}

void WxTextPage::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(m_code = new ee0::WxCodeCtrl(this, "input"), 3, wxEXPAND);

	auto btn = new wxButton(this, wxID_ANY, "Run");
	Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(WxTextPage::OnRunPress));
	sizer->Add(btn, 0, wxALIGN_CENTER_HORIZONTAL);

	SetSizer(sizer);
}

void WxTextPage::OnRunPress(wxCommandEvent& event)
{
    auto str = m_code->GetText().ToStdString();
    printf("Run:\n%s\n", str.c_str());
}

}