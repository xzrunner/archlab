#pragma once

#include "cgaview/Node.h"

#include <cga/op/Comp.h>
#include <cga/op/Extrude.h>
#include <cga/op/Offset.h>
#include <cga/op/Split.h>
#include <cga/RelativeFloat.h>

namespace cgav
{

void nodes_regist_rttr();

namespace node
{

// creation

#define ExtrusionType cga::op::Extrude::ExtrusionType
#define PARM_FILEPATH "cga/op/Extrude.parm.h"
#define PARM_NODE_CLASS Extrude
#define PARM_NODE_CLASS_STR "Extrude"
#define PARM_NODE_NAME "extrude"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
#undef ExtrusionType
;
#define PARM_FILEPATH "cga/op/PrimCube.parm.h"
#define PARM_NODE_CLASS PrimCube
#define PARM_NODE_CLASS_STR "PrimCube"
#define PARM_NODE_NAME "prim_cube"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
;
#define PARM_FILEPATH "cga/op/PrimQuad.parm.h"
#define PARM_NODE_CLASS PrimQuad
#define PARM_NODE_CLASS_STR "PrimQuad"
#define PARM_NODE_NAME "prim_quad"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
;

// subdivision

#define Type cga::op::Comp::Type
#define Selector cga::op::Comp::Selector
#define PARM_FILEPATH "cga/op/Comp.parm.h"
#define PARM_NODE_CLASS Comp
#define PARM_NODE_CLASS_STR "Comp"
#define PARM_NODE_NAME "comp"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
#undef Type
#undef Selector
;
#define Selector cga::op::Offset::Selector
#define PARM_FILEPATH "cga/op/Offset.parm.h"
#define PARM_NODE_CLASS Offset
#define PARM_NODE_CLASS_STR "Offset"
#define PARM_NODE_NAME "offset"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
#undef Selector
;
#define PARM_FILEPATH "cga/op/ShapeO.parm.h"
#define PARM_NODE_CLASS ShapeO
#define PARM_NODE_CLASS_STR "ShapeO"
#define PARM_NODE_NAME "shapeo"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
;
#define Axis cga::op::Split::Axis
#define Part cga::op::Split::Part
#define PARM_FILEPATH "cga/op/Split.parm.h"
#define PARM_NODE_CLASS Split
#define PARM_NODE_CLASS_STR "Split"
#define PARM_NODE_NAME "split"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
#undef Axis
#undef Part
;

// transformations

#define RelativeFloat cga::RelativeFloat
#define PARM_FILEPATH "cga/op/Scale.parm.h"
#define PARM_NODE_CLASS Scale
#define PARM_NODE_CLASS_STR "Scale"
#define PARM_NODE_NAME "s"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
#undef RelativeFloat
;

#define RelativeFloat cga::RelativeFloat
#define PARM_FILEPATH "cga/op/TransScope.parm.h"
#define PARM_NODE_CLASS TransScope
#define PARM_NODE_CLASS_STR "TransScope"
#define PARM_NODE_NAME "t"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
#undef RelativeFloat
;

// attributes

#define PARM_FILEPATH "cga/op/Color.parm.h"
#define PARM_NODE_CLASS Color
#define PARM_NODE_CLASS_STR "Color"
#define PARM_NODE_NAME "color"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
;

}

}