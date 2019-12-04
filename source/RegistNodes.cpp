#include "cgaview/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

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
rttr::registration::class_<cgav::node::PrimCube>("cgav::prim_cube")
.constructor<>()
#define PARM_FILEPATH "cga/node/PrimCube.parm.h"
#define PARM_NODE_CLASS PrimCube
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cgav::node::PrimQuad>("cgav::prim_quad")
.constructor<>()
#define PARM_FILEPATH "cga/node/PrimQuad.parm.h"
#define PARM_NODE_CLASS PrimQuad
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

}

namespace cgav
{

void nodes_regist_rttr()
{
}

}