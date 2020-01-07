#include "cgaview/WxSceneProp.h"
#include "cgaview/MessageID.h"
#include "cgaview/Scene.h"
#include "cgaview/ModelAdapter.h"

#include <ee0/SubjectMgr.h>
#include <ee0/GameObj.h>

#include <node0/SceneNode.h>
#include <cgaeasy/CompCGA.h>

#include <wx/sizer.h>
#include <wx/propgrid/propgrid.h>

namespace cgav
{

WxSceneProp::WxSceneProp(wxWindow* parent, ee0::SubjectMgr& editor_panel_sub_mgr,
                         ee0::SubjectMgr& preview_sub_mgr, const Scene& scene)
    : wxPanel(parent)
    , m_editor_panel_sub_mgr(editor_panel_sub_mgr)
    , m_preview_sub_mgr(preview_sub_mgr)
    , m_scene(scene)
{
    InitLayout();

    m_editor_panel_sub_mgr.RegisterObserver(MSG_RULE_INSERTED, this);
    m_preview_sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_INSERT, this);
    m_preview_sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_CLEAR, this);
}

WxSceneProp::~WxSceneProp()
{
    m_editor_panel_sub_mgr.UnregisterObserver(MSG_RULE_INSERTED, this);
    m_preview_sub_mgr.UnregisterObserver(ee0::MSG_NODE_SELECTION_INSERT, this);
    m_preview_sub_mgr.UnregisterObserver(ee0::MSG_NODE_SELECTION_CLEAR, this);
}

void WxSceneProp::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    switch (msg)
    {
    case MSG_RULE_INSERTED:
    {
        auto var = variants.GetVariant("name");
        assert(var.m_type == ee0::VT_PCHAR);
        m_rule_list->Append(var.m_val.pc);
    }
        break;

    case ee0::MSG_NODE_SELECTION_INSERT:
    {
        auto var_obj = variants.GetVariant("obj");
        GD_ASSERT(var_obj.m_type == ee0::VT_PVOID, "no var in vars: obj");
        const ee0::GameObj obj = *static_cast<const ee0::GameObj*>(var_obj.m_val.pv);
        GD_ASSERT(GAME_OBJ_VALID(obj), "err scene obj");

        m_selected_node = obj;
        LoadFromNode(obj);
    }
        break;

    case ee0::MSG_NODE_SELECTION_CLEAR:
    {
        m_selected_node.reset();
        m_node_prop->Clear();
    }
        break;
    }
}

void WxSceneProp::ReloadRulesList(const Scene& scene)
{
    m_rule_list->Clear();
    for (auto& rule : scene.GetAllRules()) {
        m_rule_list->Append(rule->name);
    }
}

void WxSceneProp::InitLayout()
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    m_rule_list = new wxListBox(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, 0, 0, wxLB_SINGLE);
    Connect(m_rule_list->GetId(), wxEVT_LISTBOX_DCLICK, wxCommandEventHandler(
        WxSceneProp::OnDClickRule));
    sizer->Add(m_rule_list, 1, wxEXPAND);

    m_node_prop = new wxPropertyGrid(this, -1, wxDefaultPosition, wxSize(500, -1),
        wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED
    );
    Connect(m_node_prop->GetId(), wxEVT_PG_CHANGING, wxPropertyGridEventHandler(
        WxSceneProp::OnNodePropChanging));
    Connect(m_node_prop->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(
        WxSceneProp::OnNodePropChanged));
    sizer->Add(m_node_prop, wxEXPAND);

    SetSizer(sizer);
}

void WxSceneProp::OnNodePropChanging(wxPropertyGridEvent& event)
{
}

void WxSceneProp::OnNodePropChanged(wxPropertyGridEvent& event)
{
    wxPGProperty* property = event.GetProperty();

    auto key = property->GetName();
    assert(key == "Rule");

    wxAny val = property->GetValue();
    auto idx = wxANY_AS(val, int);

    assert(m_selected_node && m_selected_node->HasUniqueComp<cgae::CompCGA>());
    auto& ccga = m_selected_node->GetUniqueComp<cgae::CompCGA>();
    if (idx == 0)
    {
        ccga.SetRule(nullptr);
        ccga.SetFilepath("");
    }
    else
    {
        assert(idx >= 1 && idx < static_cast<int>(m_scene.GetAllRules().size()) + 1);
        auto& rule = m_scene.GetAllRules()[idx - 1];
        ccga.SetRule(rule->impl);
        ccga.SetFilepath(rule->filepath);
    }

    ModelAdapter::BuildModel(*m_selected_node);

    m_preview_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxSceneProp::OnDClickRule(wxCommandEvent& event)
{
    int idx = event.GetSelection();
    auto name = m_rule_list->GetString(idx).ToStdString();

    ee0::VariantSet vars;

    ee0::Variant var_name;
    var_name.m_type = ee0::VT_PCHAR;
    char* tmp = new char[name.size() + 1];
    strcpy(tmp, name.c_str());
    var_name.m_val.pc = tmp;
    vars.SetVariant("name", var_name);

    m_editor_panel_sub_mgr.NotifyObservers(MSG_RULE_SHOW, vars);
}

void WxSceneProp::LoadFromNode(const n0::SceneNodePtr& node)
{
    m_node_prop->Clear();

    if (!node->HasUniqueComp<cgae::CompCGA>()) {
        return;
    }

    auto& ccga = node->GetUniqueComp<cgae::CompCGA>();
    auto& rule = ccga.GetRule();

    wxArrayString rule_choices;
    auto& rules = m_scene.GetAllRules();
    rule_choices.reserve(rules.size() + 1);
    rule_choices.push_back("Null");
    int selected = 0;
    for (size_t i = 0, n = rules.size(); i < n; ++i)
    {
        rule_choices.push_back(rules[i]->name);
        if (rules[i]->impl == rule) {
            selected = i + 1;
        }
    }

    auto rule_prop = new wxEnumProperty("Rule", wxPG_LABEL, rule_choices);
    rule_prop->SetValue(selected);
    m_node_prop->Append(rule_prop);

    m_preview_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}