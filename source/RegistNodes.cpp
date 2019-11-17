#include "cgaview/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

#define REGIST_NODE_RTTI(type, name, prop)                      \
	rttr::registration::class_<cgav::node::type>("cgav::"#name) \
		.constructor<>()                                        \
		prop                                                    \
	;

#define PROP
#define REGIST_NODE_RTTI_DEFAULT(type, name) REGIST_NODE_RTTI(type, name, PROP)

RTTR_REGISTRATION
{

// base
rttr::registration::class_<cgav::Node>("cgav::node")
.property("name", &cgav::Node::GetName, &cgav::Node::SetName)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Name"))
)
.property("display", &cgav::Node::GetDisplay, &cgav::Node::SetDisplay)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo(cgav::Node::STR_PROP_DISPLAY))
)
;

// creation
REGIST_NODE_RTTI(PrimCube, prim_cube,
.property("size", &cgav::node::PrimCube::size)                    \
(                                                                 \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Size")) \
)                                                                 \
)

}

namespace cgav
{

void nodes_regist_rttr()
{
}

}