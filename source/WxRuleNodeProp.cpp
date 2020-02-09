#include "cev/WxRuleNodeProp.h"
#include "cev/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

namespace cev
{

WxRuleNodeProp::WxRuleNodeProp(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr)
    : bp::WxNodeProperty(parent, sub_mgr)
{
}

bool WxRuleNodeProp::InitView(const rttr::property& prop, const bp::NodePtr& node)
{
    bool ret = false;

    auto ui_info_obj = prop.get_metadata(ee0::UIMetaInfoTag());
    if (!ui_info_obj.is_valid()) {
        return ret;
    }

    auto ui_info = ui_info_obj.get_value<ee0::UIMetaInfo>();
    auto node_type = node->get_type();
    auto prop_type = prop.get_type();
    if (prop_type.is_sequential_container())
    {
        if (prop_type == rttr::type::get<std::vector<ce::op::Comp::Selector>>())
        {
            wxArrayString all_items, selected_items;

            auto type = rttr::type::get<ce::op::Comp::Selector>();
            auto vars = type.get_enumeration().get_values();
            all_items.resize(vars.size());
            for (auto& var : vars)
            {
                auto idx = var.to_int();
                auto desc = type.get_enumeration().get_metadata(idx);
                assert(desc.is_valid());
                all_items[idx] = desc.to_string();
            }

            assert(node->get_type() == rttr::type::get<node::Comp>());

            auto comp_node = std::static_pointer_cast<node::Comp>(node);
            for (auto& sel : comp_node->m_selectors)
            {

                bool find = false;
                for (auto var : vars)
                {
                    if (var.get_value<ce::op::Comp::Selector>() == sel)
                    {
                        auto desc = type.get_enumeration().get_metadata(var.to_int());
                        selected_items.push_back(desc.to_string());
                        find = true;
                        break;
                    }
                }
                assert(find);
            }

            auto wx_prop = new wxMultiChoiceProperty(ui_info.desc, wxPG_LABEL, all_items, selected_items);
            m_pg->Append(wx_prop);

            ret = true;
        }
    }
    return ret;
}

bool WxRuleNodeProp::UpdateView(const rttr::property& prop, const wxPGProperty& wx_prop)
{
    bool ret = false;

    auto ui_info_obj = prop.get_metadata(ee0::UIMetaInfoTag());
    if (!ui_info_obj.is_valid()) {
        return ret;
    }

    auto key = wx_prop.GetName();
    wxAny val = wx_prop.GetValue();

    auto ui_info = ui_info_obj.get_value<ee0::UIMetaInfo>();
    auto node_type = m_node->get_type();
    auto prop_type = prop.get_type();

    if (prop_type.is_sequential_container() && key == ui_info.desc)
    {
        if (prop_type == rttr::type::get<std::vector<ce::op::Comp::Selector>>())
        {
            std::vector<ce::op::Comp::Selector> dst;
            auto src = wxANY_AS(val, wxArrayString);
            dst.reserve(src.size());

            auto enum_type = rttr::type::get<ce::op::Comp::Selector>();
            auto enum_vars = enum_type.get_enumeration().get_values();
            auto enum_prop = rttr::type::get<ce::op::Comp::Selector>().get_enumeration();
            for (auto& s : src)
            {
                auto label = s.ToStdString();

                bool find = false;
                for (auto var : enum_vars)
                {
                    auto idx = var.to_int();
                    if (enum_type.get_enumeration().get_metadata(idx) == label)
                    {
                        dst.push_back(var.get_value<ce::op::Comp::Selector>());
                        find = true;
                        break;
                    }
                }
                assert(find);
            }
            prop.set_value(m_node, dst);

            ret = true;
        }
    }

    return ret;
}

bool WxRuleNodeProp::UpdateView(wxPropertyGridEvent& event)
{
    // todo
    return false;
}

}