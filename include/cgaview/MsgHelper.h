#pragma once

#include <string>

namespace ee0 { class SubjectMgr; }
namespace cga { class EvalRule; }

namespace cgav
{

class MsgHelper
{
public:
    static void RuleChanged(ee0::SubjectMgr& sub_mgr,
        const std::string& filepath, const std::shared_ptr<cga::EvalRule>& rule);

}; // MsgHelper

}