#pragma once

#include <string>

namespace ee0 { class SubjectMgr; }
namespace ce { class EvalRule; class EvalContext; }

namespace cev
{

class MsgHelper
{
public:
    static void RuleChanged(ee0::SubjectMgr& sub_mgr, const std::string& filepath,
        const std::shared_ptr<ce::EvalRule>& rule, const std::shared_ptr<ce::EvalContext>& rule_ctx);

}; // MsgHelper

}