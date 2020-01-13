#pragma once

#include <cga/typedef.h>
#include <cga/OpVarType.h>

namespace cga { class Node; }
namespace bp { class Node; class Pin; }

namespace cgav
{

class Evaluator;

class CGAAdapter
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        cga::Operation& back, const Evaluator& eval);

    static cga::OpPtr CreateBackFromFront(const bp::Node& front);

    static int TypeBackToFront(cga::OpVarType type);

}; // SOP

}