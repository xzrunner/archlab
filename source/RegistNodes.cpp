#include "cev/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

RTTR_REGISTRATION
{

// base

rttr::registration::class_<cev::Node>("cev::node")
.property("name", &cev::Node::GetName, &cev::Node::SetName)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Name"))
)
.property("display", &cev::Node::GetDisplay, &cev::Node::SetDisplay)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo(cev::Node::STR_PROP_DISPLAY))
)
;

#define EXE_FILEPATH "cev/node_rttr_gen.h"
#include "cev/node_regist_cfg.h"
#undef EXE_FILEPATH

}

namespace cev
{

void nodes_regist_rttr()
{
}

}