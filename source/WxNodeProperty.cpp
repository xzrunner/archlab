#include "cgaview/WxNodeProperty.h"
#include "cgaview/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

#include <cga/node/SetSize.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace cgav
{

WxNodeProperty::WxNodeProperty(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr)
    : bp::WxNodeProperty(parent, sub_mgr)
{
}

bool WxNodeProperty::InitView(const rttr::property& prop, const bp::NodePtr& node)
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
        if (prop_type == rttr::type::get<std::vector<cga::node::Comp::Selector>>())
        {
            wxArrayString all_items, selected_items;

            auto type = rttr::type::get<cga::node::Comp::Selector>();
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
                    if (var.get_value<cga::node::Comp::Selector>() == sel)
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
    else if (prop_type == rttr::type::get<cga::node::SetSize::SizeValue>())
    {
        auto v = prop.get_value(node).get_value<cga::node::SetSize::SizeValue>();

        wxPGProperty* prop = m_pg->Append(new wxStringProperty(ui_info.desc, wxPG_LABEL, wxT("<composed>")));
        prop->SetExpanded(false);

        m_pg->AppendIn(prop, new wxFloatProperty(wxT("Value"), wxPG_LABEL, v.value));

        auto c_prop = m_pg->AppendIn(prop, new wxBoolProperty(wxT("Relative"), wxPG_LABEL, v.relative));
        m_pg->SetPropertyAttribute(c_prop, wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
    }

    return ret;
}

bool WxNodeProperty::UpdateView(const rttr::property& prop, const wxPGProperty& wx_prop)
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
        if (prop_type == rttr::type::get<std::vector<cga::node::Comp::Selector>>())
        {
            std::vector<cga::node::Comp::Selector> dst;
            auto src = wxANY_AS(val, wxArrayString);
            dst.reserve(src.size());

            auto enum_type = rttr::type::get<cga::node::Comp::Selector>();
            auto enum_vars = enum_type.get_enumeration().get_values();
            auto enum_prop = rttr::type::get<cga::node::Comp::Selector>().get_enumeration();
            for (auto& s : src)
            {
                auto label = s.ToStdString();

                bool find = false;
                for (auto var : enum_vars)
                {
                    auto idx = var.to_int();
                    if (enum_type.get_enumeration().get_metadata(idx) == label)
                    {
                        dst.push_back(var.get_value<cga::node::Comp::Selector>());
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
    else if (prop_type == rttr::type::get<cga::node::SetSize::SizeValue>() && key == ui_info.desc)
    {
        std::vector<std::string> tokens;
        auto str = wxANY_AS(val, wxString).ToStdString();
        boost::split(tokens, str, boost::is_any_of(";"));
        assert(tokens.size() == 2);

        auto v = prop.get_value(m_node).get_value<cga::node::SetSize::SizeValue>();
        v.value = std::stof(tokens[0]);
        if (tokens[1] == " Relative") {
            v.relative = true;
        } else if (tokens[1] == " Not Relative") {
            v.relative = false;
        } else {
            assert(0);
        }
        prop.set_value(m_node, v);

        ret = true;
    }

    return ret;
}

bool WxNodeProperty::UpdateView(wxPropertyGridEvent& event)
{
    // todo
    return false;
}

}