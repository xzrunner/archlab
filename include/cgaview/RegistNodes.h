#pragma once

#include "cgaview/Node.h"

#include <cga/node/Comp.h>
#include <cga/node/Extrude.h>
#include <cga/node/Offset.h>
#include <cga/node/Split.h>
#include <cga/RelativeFloat.h>

namespace cgav
{

void nodes_regist_rttr();

namespace node
{

// creation

#define ExtrusionType cga::node::Extrude::ExtrusionType
#define PARM_FILEPATH "cga/node/Extrude.parm.h"
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
#define PARM_FILEPATH "cga/node/PrimCube.parm.h"
#define PARM_NODE_CLASS PrimCube
#define PARM_NODE_CLASS_STR "PrimCube"
#define PARM_NODE_NAME "prim_cube"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
;
#define PARM_FILEPATH "cga/node/PrimQuad.parm.h"
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

#define Type cga::node::Comp::Type
#define Selector cga::node::Comp::Selector
#define PARM_FILEPATH "cga/node/Comp.parm.h"
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
#define Selector cga::node::Offset::Selector
#define PARM_FILEPATH "cga/node/Offset.parm.h"
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
#define PARM_FILEPATH "cga/node/ShapeO.parm.h"
#define PARM_NODE_CLASS ShapeO
#define PARM_NODE_CLASS_STR "ShapeO"
#define PARM_NODE_NAME "shapeo"
#include "cgaview/node_def_gen.h"
#undef PARM_NODE_NAME
#undef PARM_NODE_CLASS
#undef PARM_NODE_CLASS_STR
#undef PARM_FILEPATH
;
#define Axis cga::node::Split::Axis
#define Part cga::node::Split::Part
#define PARM_FILEPATH "cga/node/Split.parm.h"
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
#define PARM_FILEPATH "cga/node/Scale.parm.h"
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
#define PARM_FILEPATH "cga/node/TransScope.parm.h"
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

#define PARM_FILEPATH "cga/node/Color.parm.h"
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