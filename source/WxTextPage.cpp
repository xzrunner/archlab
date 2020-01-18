#include "cgaview/WxTextPage.h"
#include "cgaview/MsgHelper.h"
#include "cgaview/ModelAdapter.h"

#include <ee0/WxCodeCtrl.h>

#include <cga/EvalRule.h>
#include <cga/RuleLoader.h>
#include <cga/EvalContext.h>
#include <cgac/StringPool.h>
#include <cgaeasy/CompCGA.h>
#include <node0/SceneNode.h>
#include <ns/NodeFactory.h>

#include <wx/sizer.h>
#include <wx/button.h>

namespace cgav
{

WxTextPage::WxTextPage(wxWindow* parent, Scene& scene,
                       const ee0::SubjectMgrPtr& preview_sub_mgr)
    : wxPanel(parent)
    , m_scene(scene)
    , m_preview_sub_mgr(preview_sub_mgr)
    , m_str_pool(std::make_shared<cgac::StringPool>())
{
    m_preview_obj = ns::NodeFactory::Create3D();
    ModelAdapter::SetupModel(*m_preview_obj);
    m_preview_obj->AddUniqueComp<cgae::CompCGA>();

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

    m_eval_ctx = std::make_shared<cga::EvalContext>();

    cga::RuleLoader loader(m_str_pool);
    if (!m_eval) {
        m_eval = std::make_shared<cga::EvalRule>();
    } else {
        m_eval->Clear();
    }
    loader.RunString(*m_eval_ctx, str, *m_eval/*, true*/);

    MsgHelper::RuleChanged(*m_preview_sub_mgr, m_rule_path, m_eval, m_eval_ctx);
}

}