#include "cgaview/WxTextPage.h"
#include "cgaview/MessageID.h"

#include <ee0/WxCodeCtrl.h>
#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <cga/EvalRule.h>
#include <cga/RuleLoader.h>

#include <wx/sizer.h>
#include <wx/button.h>

namespace cgav
{

WxTextPage::WxTextPage(wxWindow* parent, Scene& scene,
                       const ee0::SubjectMgrPtr& preview_sub_mgr)
    : wxPanel(parent)
    , m_scene(scene)
    , m_preview_sub_mgr(preview_sub_mgr)
{
    InitLayout();
}

std::string WxTextPage::GetText() const
{
    return m_code->GetText().ToStdString();
}

void WxTextPage::SetText(const std::string& text)
{
    m_code->SetText(text);

    RebuildEval();
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
    RebuildEval();
}

void WxTextPage::RebuildEval()
{
    auto str = m_code->GetText().ToStdString();

    cga::RuleLoader loader;
    if (!m_eval) {
        m_eval = std::make_shared<cga::EvalRule>();
    } else {
        m_eval->Clear();
    }
    loader.RunString(str, *m_eval/*, true*/);

    ee0::VariantSet vars;

    ee0::Variant var_filepath;
    var_filepath.m_type = ee0::VT_PCHAR;
    char* tmp = new char[m_rule_path.size() + 1];
    strcpy(tmp, m_rule_path.c_str());
    var_filepath.m_val.pc = tmp;
    vars.SetVariant("filepath", var_filepath);

    ee0::Variant var_rule;
    var_rule.m_type = ee0::VT_PVOID;
    var_rule.m_val.pv = &m_eval;
    vars.SetVariant("rule", var_rule);

    m_preview_sub_mgr->NotifyObservers(MSG_RULE_CHANGED, vars);
}

}