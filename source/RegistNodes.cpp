#include "archlab/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

RTTR_REGISTRATION
{

// base

rttr::registration::class_<archlab::Node>("archlab::node")
.property("name", &archlab::Node::GetName, &archlab::Node::SetName)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Name"))
)
.property("display", &archlab::Node::GetDisplay, &archlab::Node::SetDisplay)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo(archlab::Node::STR_PROP_DISPLAY))
)
;

#define EXE_FILEPATH "archlab/node_rttr_gen.h"
#include "archlab/node_regist_cfg.h"
#undef EXE_FILEPATH

}

namespace archlab
{

void nodes_regist_rttr()
{
}

}