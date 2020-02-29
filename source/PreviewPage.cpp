#include "archlab/PreviewPage.h"
#include "archlab/MessageID.h"
#include "archlab/ModelAdapter.h"
#include "archlab/MessageID.h"
#include "archlab/WxGraphPage.h"
#include "archlab/WxTextPage.h"
#include "archlab/Evaluator.h"
#include "archlab/MessageID.h"
#include "archlab/Scene.h"

#include <ee0/SubjectMgr.h>
#include <ee0/GameObj.h>
#include <ee0/WxStagePage.h>
#include <ee3/WxStageCanvas.h>
#include <ee3/WorldTravelOP.h>
#include <ee3/CameraDriveOP.h>
#include <draft3/PolygonBuildOP.h>
#include <draft3/PolygonSelectOP.h>

#include <guard/check.h>
#include <easyarchgraph/EasyArchGraph.h>
#include <easyarchgraph/CompArchGraph.h>
#include <node0/SceneNode.h>

namespace archlab
{

PreviewPage::PreviewPage(ee0::WxStagePage& stage_page)
    : m_stage_page(stage_page)
{
    m_messages.push_back(ee0::MSG_SCENE_NODE_INSERT);
    m_messages.push_back(MSG_SET_EDIT_OP);
    m_messages.push_back(MSG_SET_SELECT_OP);
    m_messages.push_back(MSG_RULE_CHANGED);
    for (auto& msg : m_messages) {
        stage_page.GetSubjectMgr()->RegisterObserver(msg, this);
    }

    easyarchgraph::EasyArchGraph::Init();
}

PreviewPage::~PreviewPage()
{
    for (auto& msg : m_messages) {
        m_stage_page.GetSubjectMgr()->UnregisterObserver(msg, this);
    }
}

void PreviewPage::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    switch (msg)
    {
    case ee0::MSG_SCENE_NODE_INSERT:
    {
        auto var = variants.GetVariant("obj");
        GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
        const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
        GD_ASSERT(obj, "err scene obj");

        if (!(*obj)->HasUniqueComp<easyarchgraph::CompArchGraph>()) {
            (*obj)->AddUniqueComp<easyarchgraph::CompArchGraph>();
        }
    }
        break;

    case MSG_SET_EDIT_OP:
        m_stage_page.GetImpl().SetEditOP(m_edit_op);
        break;
    case MSG_SET_SELECT_OP:
        m_stage_page.GetImpl().SetEditOP(m_select_op);
        break;

    case MSG_RULE_CHANGED:
    {
        m_stage_page.GetSubjectMgr()->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

        auto var_filepath = variants.GetVariant("filepath");
        GD_ASSERT(var_filepath.m_type == ee0::VT_PCHAR, "err var");
        std::string rule_path = static_cast<const char*>(var_filepath.m_val.pc);

        auto var_rule = variants.GetVariant("rule");
        GD_ASSERT(var_rule.m_type == ee0::VT_PVOID, "err var");
        const std::shared_ptr<archgraph::EvalRule>* rule
            = static_cast<const std::shared_ptr<archgraph::EvalRule>*>(var_rule.m_val.pv);

        auto var_ctx = variants.GetVariant("ctx");
        GD_ASSERT(var_ctx.m_type == ee0::VT_PVOID, "err var");
        const std::shared_ptr<archgraph::EvalContext>* ctx
            = static_cast<const std::shared_ptr<archgraph::EvalContext>*>(var_rule.m_val.pv);

        m_stage_page.Traverse([&](const ee0::GameObj& obj)->bool
        {
            if (!obj->HasUniqueComp<easyarchgraph::CompArchGraph>()) {
                return true;
            }

            auto& ccga = obj->GetUniqueComp<easyarchgraph::CompArchGraph>();
            if (ccga.GetFilepath() == rule_path)
            {
                auto& ccga = obj->GetUniqueComp<easyarchgraph::CompArchGraph>();
                if (ccga.GetRule() != *rule) {
                    ccga.SetRule(*rule, *ctx);
                }
                ModelAdapter::BuildModel(*obj);
            }

            return true;
        });
    }
        break;
    }
}

void PreviewPage::InitEditOP()
{
    auto canvas = m_stage_page.GetImpl().GetCanvas();
    assert(canvas);
    auto sub_mgr = m_stage_page.GetSubjectMgr();
    assert(sub_mgr);

    auto& vp = std::static_pointer_cast<ee3::WxStageCanvas>(canvas)->GetViewport();
    m_select_op = std::make_shared<draft3::PolygonSelectOP>(
        canvas->GetCamera(), m_stage_page, vp
    );
    m_edit_op = std::make_shared<draft3::PolygonBuildOP>(
        canvas->GetCamera(), vp, sub_mgr
    );
    m_select_op->SetPrevEditOP(std::make_shared<ee3::WorldTravelOP>(
        canvas->GetCamera(), vp, sub_mgr
    ));
    m_edit_op->SetPrevEditOP(std::make_shared<ee3::CameraDriveOP>(
        canvas->GetCamera(), vp, sub_mgr
    ));
    m_stage_page.GetImpl().SetEditOP(m_edit_op);
}

void PreviewPage::InitSceneNodeRule(const Scene& scene)
{
    m_stage_page.Traverse([&](const ee0::GameObj& obj)->bool
    {
        if (!obj->HasUniqueComp<easyarchgraph::CompArchGraph>()) {
            return true;
        }

        auto& ccga = obj->GetUniqueComp<easyarchgraph::CompArchGraph>();
        for (auto& rule : scene.GetAllRules()) {
            if (rule->filepath == ccga.GetFilepath()) {
                ccga.SetRule(rule->impl, rule->ctx);
                break;
            }
        }

        ModelAdapter::BuildModel(*obj);

        return true;
    });
}

}