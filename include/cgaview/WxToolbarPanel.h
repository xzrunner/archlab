#pragma once

#include <ee0/Observer.h>
#include <ee0/typedef.h>

#include <wx/panel.h>

namespace ee0 { class WxStageCanvas; class WxNavigationBar; }
namespace bp { class WxNodeProperty; }
namespace cga { class EvalContext; }

namespace cgav
{

class WxRuleProperty;

class WxToolbarPanel : public wxPanel, public ee0::Observer
{
public:
	WxToolbarPanel(wxWindow* parent, cga::EvalContext& ctx,
        const ee0::SubjectMgrPtr& graph_sub_mgr, const ee0::SubjectMgrPtr& preview_sub_mgr);

	virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

private:
	void InitLayout(cga::EvalContext& ctx, const ee0::SubjectMgrPtr& graph_sub_mgr);

    void OnChangeEditType(wxCommandEvent& event);

    void OnSelectionInsert(const ee0::VariantSet& variants);
    void OnSelectionClear(const ee0::VariantSet& variants);

private:
    ee0::SubjectMgrPtr m_preview_sub_mgr = nullptr;

    WxRuleProperty*     m_rule_prop;
    bp::WxNodeProperty* m_rule_node_prop;

}; // WxToolbarPanel

}