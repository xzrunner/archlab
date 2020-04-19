#pragma once

#include <blueprint/WxNodeProperty.h>

namespace archlab
{

class WxRuleNodeProp : public bp::WxNodeProperty
{
public:
    WxRuleNodeProp(const ur2::Device& dev, wxWindow* parent,
        const ee0::SubjectMgrPtr& sub_mgr);

protected:
    virtual bool InitView(const rttr::property& prop, const bp::NodePtr& node) override;
    virtual bool UpdateView(const rttr::property& prop, const wxPGProperty& wx_prop) override;
    virtual bool UpdateView(wxPropertyGridEvent& event) override;

}; // WxRuleNodeProp

}