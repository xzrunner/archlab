#pragma once

#include <string>

namespace ee0 { class SubjectMgr; }
namespace archgraph { class EvalRule; class EvalContext; }

namespace archlab
{

class MsgHelper
{
public:
    static void RuleChanged(ee0::SubjectMgr& sub_mgr, const std::string& filepath,
        const std::shared_ptr<archgraph::EvalRule>& rule, const std::shared_ptr<archgraph::EvalContext>& rule_ctx);

}; // MsgHelper

}