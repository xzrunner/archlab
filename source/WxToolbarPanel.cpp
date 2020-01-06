#include "cgaview/WxToolbarPanel.h"
#include "cgaview/Node.h"
#include "cgaview/WxRuleNodeProp.h"
#include "cgaview/WxRuleProperty.h"
#include "cgaview/MessageID.h"
#include "cgaview/WxEditorPanel.h"
#include "cgaview/WxSceneProp.h"

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

void WxToolbarPanel::SetEditorPanel(WxEditorPanel* editor_panel)
{
    m_editor_panel = editor_panel;

    assert(m_preview_sub_mgr);
    m_scene_prop = new WxSceneProp(
        m_prop_nb, m_editor_panel->GetSubMgr(),
        *m_preview_sub_mgr, m_editor_panel->GetScene()
    );
    m_prop_nb->AddPage(m_scene_prop, "Scene");
}

void WxToolbarPanel::ReloadRulesList()
{
    m_scene_prop->ReloadRulesList(m_editor_panel->GetScene());
}

void WxToolbarPanel::InitLayout(cga::EvalContext& ctx, const ee0::SubjectMgrPtr& graph_sub_mgr)
{
	auto sizer = new wxBoxSizer(wxVERTICAL);

    // header
    auto header_sizer = new wxBoxSizer(wxHORIZONTAL);
    InitOperatorPanel(header_sizer);
    header_sizer->AddSpacer(20);
    InitButtonsPanel(header_sizer);
    sizer->Add(header_sizer);

    // property
    InitPropsPanel(sizer, ctx, graph_sub_mgr);

	SetSizer(sizer);
}

void WxToolbarPanel::InitOperatorPanel(wxSizer* sizer)
{
    wxArrayString choices;
    choices.Add("Edit");
    choices.Add("Select");
    auto editop = new wxRadioBox(this, wxID_ANY, "operator",
        wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_ROWS);
    Connect(editop->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED,
        wxCommandEventHandler(WxToolbarPanel::OnChangeEditType));
    editop->SetSelection(0);
    sizer->Add(editop);
}

void WxToolbarPanel::InitButtonsPanel(wxSizer* sizer)
{
    wxStaticBox* rule_bounding = new wxStaticBox(this, wxID_ANY, "rule");
    wxSizer* rule_sizer = new wxStaticBoxSizer(rule_bounding, wxHORIZONTAL);

    auto save_rule_btn = new wxButton(this, wxID_ANY, "Save...");
    Connect(save_rule_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(WxToolbarPanel::OnSaveRule));
    rule_sizer->Add(save_rule_btn);

    rule_sizer->AddSpacer(5);

    auto load_rule_btn = new wxButton(this, wxID_ANY, "Load...");
    Connect(load_rule_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(WxToolbarPanel::OnLoadRule));
    rule_sizer->Add(load_rule_btn);

    sizer->Add(rule_sizer);
}

void WxToolbarPanel::InitPropsPanel(wxSizer* sizer, cga::EvalContext& ctx,
                                    const ee0::SubjectMgrPtr& graph_sub_mgr)
{
    m_prop_nb = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
    m_prop_nb->AddPage(m_rule_node_prop = new WxRuleNodeProp(m_prop_nb, graph_sub_mgr), "Rule Node");
    m_prop_nb->AddPage(m_rule_prop = new WxRuleProperty(m_prop_nb, ctx), "Rule");
	sizer->Add(m_prop_nb, 1, wxEXPAND);
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

void WxToolbarPanel::OnSaveRule(wxCommandEvent& event)
{
    assert(m_editor_panel);
    const auto filter = m_editor_panel->IsCurrGraphPage() ? "*.json" : "*.cga";
    wxFileDialog dlg(this, wxT("Save Rule"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
    if (dlg.ShowModal() == wxID_OK) {
        m_editor_panel->SaveRuleToFile(dlg.GetPath().ToStdString());
    }
}

void WxToolbarPanel::OnLoadRule(wxCommandEvent& event)
{
    assert(m_editor_panel);
    const auto filter = m_editor_panel->IsCurrGraphPage() ? "*.json" : "*.cga";
    wxFileDialog dlg(this, wxT("Open Rule"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
    if (dlg.ShowModal() == wxID_OK) {
        m_editor_panel->LoadRuleFromFile(dlg.GetPath().ToStdString());
    }
}

}