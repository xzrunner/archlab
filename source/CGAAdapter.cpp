#include "cev/CGAAdapter.h"
#include "cev/RegistNodes.h"
#include "cev/PinType.h"
#include "cev/Node.h"

#include <blueprint/Node.h>
#include <blueprint/Pin.h>

#include <ce/Operation.h>

namespace cev
{

void CGAAdapter::UpdatePropBackFromFront(const bp::Node& front, ce::Operation& back,
                                         const Evaluator& eval)
{
    auto f_type = front.get_type();
    auto b_type = back.get_type();
    if (f_type.is_derived_from<Node>() &&
        b_type.is_derived_from<ce::Operation>())
    {
        for (auto& dst_prop : b_type.get_properties())
        {
            auto src_prop = f_type.get_property(dst_prop.get_name());
            assert(src_prop.is_valid());
            dst_prop.set_value(back, src_prop.get_value(front));

            if (b_type == rttr::type::get<ce::op::Comp>() &&
                dst_prop.get_type() == rttr::type::get<std::vector<ce::op::Comp::Selector>>()) {
                static_cast<ce::op::Comp&>(back).SetupExports();
            }
        }
    }
}

ce::OpPtr CGAAdapter::CreateBackFromFront(const bp::Node& node)
{
    auto type = node.get_type();
    auto src_type = type.get_name().to_string();
    std::string dst_type;
    std::string lib_str = "ce";
    auto find_lib = src_type.find("cev::");
    if (find_lib != std::string::npos) {
        dst_type = lib_str + "::" + src_type.substr(find_lib + strlen("cev::"));
    }

    ce::OpPtr dst = nullptr;

    if (!dst_type.empty())
    {
	    rttr::type t = rttr::type::get_by_name(dst_type);
        // fixme: specify node type
	    if (!t.is_valid())
        {
            assert(0);
	    }
        else
        {
            rttr::variant var = t.create();
            assert(var.is_valid());

            dst = var.get_value<std::shared_ptr<ce::Operation>>();
            assert(dst);
        }
    }

    if (node.get_type().is_derived_from<Node>()) {
        dst->SetName(static_cast<const Node&>(node).GetName());
    }

    return dst;
}


int CGAAdapter::TypeBackToFront(ce::OpVarType type)
{
    int ret = -1;

    switch (type)
    {
    case ce::OpVarType::Any:
        ret = bp::PIN_ANY_VAR;
        break;
    case ce::OpVarType::Primitive:
        ret = PIN_PRIMITIVE;
        break;
    }

    return ret;
}

}