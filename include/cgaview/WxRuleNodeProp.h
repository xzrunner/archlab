#pragma once

#include <blueprint/WxNodeProperty.h>

namespace cgav
{

class WxRuleNodeProp : public bp::WxNodeProperty
{
public:
    WxRuleNodeProp(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr);

protected:
    virtual bool InitView(const rttr::property& prop, const bp::NodePtr& node) override;
    virtual bool UpdateView(const rttr::property& prop, const wxPGProperty& wx_prop) override;
    virtual bool UpdateView(wxPropertyGridEvent& event) override;

}; // WxRuleNodeProp

}