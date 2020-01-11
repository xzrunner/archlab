#include "cgaview/MsgHelper.h"
#include "cgaview/MessageID.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

namespace cgav
{

void MsgHelper::RuleChanged(ee0::SubjectMgr& sub_mgr, const std::string& filepath,
                            const std::shared_ptr<cga::EvalRule>& rule)
{
    if (filepath.empty()) {
        return;
    }

    ee0::VariantSet vars;

    ee0::Variant var_filepath;
    var_filepath.m_type = ee0::VT_PCHAR;
    char* tmp = new char[filepath.size() + 1];
    strcpy(tmp, filepath.c_str());
    var_filepath.m_val.pc = tmp;
    vars.SetVariant("filepath", var_filepath);

    ee0::Variant var_rule;
    var_rule.m_type = ee0::VT_PVOID;
    var_rule.m_val.pv = &rule;
    vars.SetVariant("rule", var_rule);

    sub_mgr.NotifyObservers(MSG_RULE_CHANGED, vars);
}

}