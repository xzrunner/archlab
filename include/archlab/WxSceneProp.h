#pragma once

#include <ee0/Observer.h>
#include <ee0/typedef.h>

#include <node0/typedef.h>

#include <wx/panel.h>
#include <wx/listbox.h>

class wxPropertyGrid;
class wxPropertyGridEvent;

namespace ur2 { class Device; }

namespace archlab
{

class Scene;

class WxSceneProp : public wxPanel, public ee0::Observer
{
public:
    WxSceneProp(const ur2::Device& dev, wxWindow* parent, ee0::SubjectMgr& editor_panel_sub_mgr,
        ee0::SubjectMgr& preview_sub_mgr, const Scene& scene);
    virtual ~WxSceneProp();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    void ReloadRulesList(const Scene& scene);

private:
    void InitLayout();

    void OnNodePropChanging(wxPropertyGridEvent& event);
    void OnNodePropChanged(wxPropertyGridEvent& event);

    void OnDClickRule(wxCommandEvent& event);

    void LoadFromNode(const n0::SceneNodePtr& node);

private:
    const ur2::Device& m_dev;

    ee0::SubjectMgr& m_editor_panel_sub_mgr;
    ee0::SubjectMgr& m_preview_sub_mgr;
    const Scene& m_scene;

    wxListBox* m_rule_list = nullptr;

    wxPropertyGrid* m_node_prop = nullptr;

    n0::SceneNodePtr m_selected_node = nullptr;

}; // WxSceneProp

}