#pragma once

#include "cgaview/Node.h"

namespace cgav
{

void nodes_regist_rttr();

namespace node
{

// creation
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

}

}