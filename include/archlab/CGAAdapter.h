#pragma once

#include <archgraph/typedef.h>
#include <archgraph/OpVarType.h>

namespace archgraph { class Node; }
namespace bp { class Node; class Pin; }

namespace archlab
{

class Evaluator;

class CGAAdapter
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        archgraph::Operation& back, const Evaluator& eval);

    static archgraph::OpPtr CreateBackFromFront(const bp::Node& front);

    static int TypeBackToFront(archgraph::OpVarType type);

}; // SOP

}