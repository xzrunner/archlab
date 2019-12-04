#pragma once

#include <cga/typedef.h>
#include <cga/NodeVarType.h>

namespace cga { class Node; }
namespace bp { class Node; class Pin; }

namespace cgav
{

class Evaluator;

class CGAAdapter
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        cga::Node& back, const Evaluator& eval);

    static cga::NodePtr CreateBackFromFront(const bp::Node& front);

    static int TypeBackToFront(cga::NodeVarType type);

}; // SOP

}