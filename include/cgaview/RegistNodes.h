#pragma once

#include "cgaview/Node.h"

#include <cga/node/Extrude.h>
#include <cga/node/Offset.h>

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