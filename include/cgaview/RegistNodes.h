#pragma once

#include "cgaview/Node.h"

#include <cga/op/Comp.h>
#include <cga/op/Extrude.h>
#include <cga/op/Insert.h>
#include <cga/op/Offset.h>
#include <cga/op/Split.h>
#include <cga/RelativeFloat.h>

namespace cgav
{

void nodes_regist_rttr();

namespace node
{

#define EXE_FILEPATH "cgaview/node_def_gen.h"
#include "cgaview/node_regist_cfg.h"
#undef EXE_FILEPATH

}

}