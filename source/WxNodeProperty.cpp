#include "cgaview/WxNodeProperty.h"

namespace cgav
{

WxNodeProperty::WxNodeProperty(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr)
	: bp::WxNodeProperty(parent, sub_mgr)
{
}

bool WxNodeProperty::InitView(const rttr::property& prop, const bp::NodePtr& node)
{
    return false;
}

bool WxNodeProperty::UpdateView(const rttr::property& prop, const wxPGProperty& wx_prop)
{
    return false;
}

bool WxNodeProperty::UpdateView(wxPropertyGridEvent& event)
{
    return false;
}

}