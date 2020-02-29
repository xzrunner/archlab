#include "archlab/WxTextPage.h"
#include "archlab/MsgHelper.h"
#include "archlab/ModelAdapter.h"

#include <ee0/WxCodeCtrl.h>

#include <archgraph/EvalRule.h>
#include <archgraph/RuleLoader.h>
#include <archgraph/EvalContext.h>
#include <cga/StringPool.h>
#include <easyarchgraph/CompArchGraph.h>
#include <node0/SceneNode.h>
#include <ns/NodeFactory.h>

#include <wx/sizer.h>
#include <wx/button.h>

namespace archlab
{

WxTextPage::WxTextPage(wxWindow* parent, Scene& scene,
                       const ee0::SubjectMgrPtr& preview_sub_mgr)
    : wxPanel(parent)
    , m_scene(scene)
    , m_preview_sub_mgr(preview_sub_mgr)
    , m_str_pool(std::make_shared<cga::StringPool>())
{
    m_preview_obj = ns::NodeFactory::Create3D();
    ModelAdapter::SetupModel(*m_preview_obj);
    m_preview_obj->AddUniqueComp<easyarchgraph::CompArchGraph>();

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

    m_eval_ctx = std::make_shared<archgraph::EvalContext>();

    archgraph::RuleLoader loader(m_str_pool);
    if (!m_eval) {
        m_eval = std::make_shared<archgraph::EvalRule>();
    } else {
        m_eval->Clear();
    }
    loader.RunString(*m_eval_ctx, str, *m_eval/*, true*/);

    MsgHelper::RuleChanged(*m_preview_sub_mgr, m_rule_path, m_eval, m_eval_ctx);
}

}