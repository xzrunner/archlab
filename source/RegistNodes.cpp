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

rttr::registration::class_<cgav::node::Extrude>("cgav::extrude")
.constructor<>()
#define PARM_FILEPATH "cga/op/Extrude.parm.h"
#define PARM_NODE_CLASS Extrude
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cgav::node::PrimCube>("cgav::prim_cube")
.constructor<>()
#define PARM_FILEPATH "cga/op/PrimCube.parm.h"
#define PARM_NODE_CLASS PrimCube
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cgav::node::PrimQuad>("cgav::prim_quad")
.constructor<>()
#define PARM_FILEPATH "cga/op/PrimQuad.parm.h"
#define PARM_NODE_CLASS PrimQuad
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// subdivision

rttr::registration::class_<cgav::node::Comp>("cgav::comp")
.constructor<>()
#define PARM_FILEPATH "cga/op/Comp.parm.h"
#define PARM_NODE_CLASS Comp
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cgav::node::Offset>("cgav::offset")
.constructor<>()
#define PARM_FILEPATH "cga/op/Offset.parm.h"
#define PARM_NODE_CLASS Offset
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cgav::node::ShapeO>("cgav::shapeo")
.constructor<>()
#define PARM_FILEPATH "cga/op/ShapeO.parm.h"
#define PARM_NODE_CLASS ShapeO
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;
rttr::registration::class_<cgav::node::Split>("cgav::split")
.constructor<>()
#define PARM_FILEPATH "cga/op/Split.parm.h"
#define PARM_NODE_CLASS Split
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// transformations

rttr::registration::class_<cgav::node::Scale>("cgav::s")
.constructor<>()
#define PARM_FILEPATH "cga/op/Scale.parm.h"
#define PARM_NODE_CLASS Scale
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

rttr::registration::class_<cgav::node::TransScope>("cgav::t")
.constructor<>()
#define PARM_FILEPATH "cga/op/TransScope.parm.h"
#define PARM_NODE_CLASS TransScope
#include "cgaview/node_rttr_gen.h"
#undef PARM_NODE_CLASS
#undef PARM_FILEPATH
;

// attributes

rttr::registration::class_<cgav::node::Color>("cgav::color")
.constructor<>()
#define PARM_FILEPATH "cga/op/Color.parm.h"
#define PARM_NODE_CLASS Color
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