#pragma once

#include <ce/typedef.h>
#include <ce/OpVarType.h>

namespace ce { class Node; }
namespace bp { class Node; class Pin; }

namespace cev
{

class Evaluator;

class CGAAdapter
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        ce::Operation& back, const Evaluator& eval);

    static ce::OpPtr CreateBackFromFront(const bp::Node& front);

    static int TypeBackToFront(ce::OpVarType type);

}; // SOP

}