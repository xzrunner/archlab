#include "cgaview/WxToolbarPanel.h"
#include "cgaview/Node.h"
#include "cgaview/WxRuleNodeProp.h"
#include "cgaview/WxRuleProperty.h"
#include "cgaview/MessageID.h"

#include <ee0/WxStagePage.h>
#include <ee0/SubjectMgr.h>
#include <ee0/WxNavigationBar.h>
#include <ee3/WxStageCanvas.h>
#include <blueprint/CompNode.h>
#include <blueprint/WxNodeProperty.h>

#include <guard/check.h>
#include <node0/SceneNode.h>

#include <wx/notebook.h>

namespace cgav
{

WxToolbarPanel::WxToolbarPanel(wxWindow* parent, cga::EvalContext& ctx,
                               const ee0::SubjectMgrPtr& graph_sub_mgr,
                               const ee0::SubjectMgrPtr& preview_sub_mgr)
	: wxPanel(parent)
    , m_preview_sub_mgr(preview_sub_mgr)
{
	InitLayout(ctx, graph_sub_mgr);

    graph_sub_mgr->RegisterObserver(ee0::MSG_NODE_SELECTION_INSERT, this);
    graph_sub_mgr->RegisterObserver(ee0::MSG_NODE_SELECTION_CLEAR, this);
}

void WxToolbarPanel::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
	switch (msg)
	{
	case ee0::MSG_NODE_SELECTION_INSERT:
		OnSelectionInsert(variants);
		break;
    case ee0::MSG_NODE_SELECTION_CLEAR:
        OnSelectionClear(variants);
        break;
	}
}

void WxToolbarPanel::InitLayout(cga::EvalContext& ctx, const ee0::SubjectMgrPtr& graph_sub_mgr)
{
	auto sizer = new wxBoxSizer(wxVERTICAL);

    // operator
    wxArrayString choices;
    choices.Add("Edit");
    choices.Add("Select");
    auto editop = new wxRadioBox(this, wxID_ANY, "operator",
        wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_ROWS);
    Connect(editop->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED,
        wxCommandEventHandler(WxToolbarPanel::OnChangeEditType));
    editop->SetSelection(0);
    sizer->Add(editop);

    // property
    auto prop = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
    prop->AddPage(m_rule_node_prop = new WxRuleNodeProp(prop, graph_sub_mgr), "Rule Node");
    prop->AddPage(m_rule_prop = new WxRuleProperty(prop, ctx), "Rule");
	sizer->Add(prop, 1, wxEXPAND);

	SetSizer(sizer);
}

void WxToolbarPanel::OnChangeEditType(wxCommandEvent& event)
{
    int sel = event.GetSelection();
	switch (sel)
	{
	case 0:
        m_preview_sub_mgr->NotifyObservers(MSG_SET_EDIT_OP);
		break;
	case 1:
        m_preview_sub_mgr->NotifyObservers(MSG_SET_SELECT_OP);
        break;
	default:
		GD_REPORT_ASSERT("err type.");
	}
}

void WxToolbarPanel::OnSelectionInsert(const ee0::VariantSet& variants)
{
	auto var_obj = variants.GetVariant("obj");
	GD_ASSERT(var_obj.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj obj = *static_cast<const ee0::GameObj*>(var_obj.m_val.pv);
	GD_ASSERT(GAME_OBJ_VALID(obj), "err scene obj");

	auto& cnode = obj->GetUniqueComp<bp::CompNode>();
	m_rule_node_prop->LoadFromNode(obj, cnode.GetNode());
}

void WxToolbarPanel::OnSelectionClear(const ee0::VariantSet& variants)
{
    m_rule_node_prop->Clear();
}

}