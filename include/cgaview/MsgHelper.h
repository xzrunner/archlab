#pragma once

#include <string>

namespace ee0 { class SubjectMgr; }
namespace cga { class EvalRule; class EvalContext; }

namespace cgav
{

class MsgHelper
{
public:
    static void RuleChanged(ee0::SubjectMgr& sub_mgr, const std::string& filepath,
        const std::shared_ptr<cga::EvalRule>& rule, const std::shared_ptr<cga::EvalContext>& rule_ctx);

}; // MsgHelper

}