#pragma once

#include "cgaview/Node.h"
#

namespace cgav
{

void nodes_regist_rttr();

namespace node
{

#define CGAV_DEFINE_NODE(type, name, prop) \
class type : public Node                   \
{                                          \
public:                                    \
	type()                                 \
		: Node(#type)                      \
	{                                      \
		InitPins(#name);                   \
	}                                      \
                                           \
	prop                                   \
	RTTR_ENABLE(Node)                      \
};

// creation

CGAV_DEFINE_NODE(PrimCube, prim_cube,
    sm::vec3 size = sm::vec3(1, 1, 1); \
)

}

}