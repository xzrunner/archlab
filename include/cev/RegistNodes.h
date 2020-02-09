#pragma once

#include "cev/Node.h"

#include <ce/op/Comp.h>
#include <ce/op/Extrude.h>
#include <ce/op/Insert.h>
#include <ce/op/Offset.h>
#include <ce/op/Split.h>
#include <ce/RelativeFloat.h>

namespace cev
{

void nodes_regist_rttr();

namespace node
{

#define EXE_FILEPATH "cev/node_def_gen.h"
#include "cev/node_regist_cfg.h"
#undef EXE_FILEPATH

}

}