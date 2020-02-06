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

#define EXE_FILEPATH "cgaview/node_rttr_gen.h"
#include "cgaview/node_regist_cfg.h"
#undef EXE_FILEPATH

}

namespace cgav
{

void nodes_regist_rttr()
{
}

}