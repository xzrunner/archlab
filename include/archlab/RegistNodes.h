#pragma once

#include "archlab/Node.h"

#include <archgraph/op/Comp.h>
#include <archgraph/op/Extrude.h>
#include <archgraph/op/Insert.h>
#include <archgraph/op/Offset.h>
#include <archgraph/op/Split.h>
#include <archgraph/RelativeFloat.h>

namespace archlab
{

void nodes_regist_rttr();

namespace node
{

#define EXE_FILEPATH "archlab/node_def_gen.h"
#include "archlab/node_regist_cfg.h"
#undef EXE_FILEPATH

}

}