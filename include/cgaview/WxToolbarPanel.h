#pragma once

#include <ee0/Observer.h>

#include <wx/panel.h>

namespace ee0 { class WxStagePage; class WxNavigationBar; }

namespace cgav
{

class WxNodeProperty;

class WxToolbarPanel : public wxPanel, public ee0::Observer
{
public:
	WxToolbarPanel(wxWindow* parent, ee0::WxStagePage* stage_page);

	virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

private:
	void InitLayout();

    void OnSelectionInsert(const ee0::VariantSet& variants);
    void OnSelectionClear(const ee0::VariantSet& variants);

private:
    ee0::WxStagePage* m_stage_page;

    WxNodeProperty* m_node_prop;

}; // WxToolbarPanel

}