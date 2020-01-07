#include "cgaview/WxGraphPage.h"
#include "cgaview/WxPreviewCanvas.h"
#include "cgaview/Evaluator.h"
#include "cgaview/MessageID.h"

#include <ee0/SubjectMgr.h>
#include <ee0/WxStageCanvas.h>
#include <blueprint/MessageID.h>
#include <blueprint/CompNode.h>
#include <blueprint/Node.h>
#include <blueprint/Blueprint.h>
#include <blueprint/Pin.h>

#include <cga/EvalNode.h>
#include <node0/SceneNode.h>
#include <node0/CompComplex.h>
#include <node2/CompBoundingBox.h>

namespace
{

const uint32_t MESSAGES[] =
{
    ee0::MSG_SCENE_NODE_INSERT,
    ee0::MSG_SCENE_NODE_DELETE,
    ee0::MSG_SCENE_NODE_CLEAR,

    bp::MSG_BP_CONN_INSERT,
    bp::MSG_BP_CONN_DELETE,
    bp::MSG_BP_CONN_REBUILD,
    bp::MSG_BP_NODE_PROP_CHANGED,
};

}

namespace cgav
{

WxGraphPage::WxGraphPage(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr,
                         const ee0::GameObj& root)
    : ee0::WxStagePage(parent, sub_mgr)
    , m_root(root)
{
    bp::Blueprint::Instance();

    m_eval = std::make_shared<Evaluator>();

    for (auto& msg : MESSAGES) {
        m_sub_mgr->RegisterObserver(msg, this);
    }
}

WxGraphPage::~WxGraphPage()
{
    for (auto& msg : MESSAGES) {
        m_sub_mgr->UnregisterObserver(msg, this);
    }
}

void WxGraphPage::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    ee0::WxStagePage::OnNotify(msg, variants);

	bool dirty = false;
	switch (msg)
	{
	case ee0::MSG_SCENE_NODE_INSERT:
		dirty = InsertSceneObj(variants);
		break;
	case ee0::MSG_SCENE_NODE_DELETE:
		dirty = DeleteSceneObj(variants);
		break;
	case ee0::MSG_SCENE_NODE_CLEAR:
        dirty = ClearAllSceneObjs();
		break;

    case bp::MSG_BP_CONN_INSERT:
        dirty = AfterInsertNodeConn(variants);
        break;
    case bp::MSG_BP_CONN_DELETE:
        dirty = BeforeDeleteNodeConn(variants);
        break;
    case bp::MSG_BP_CONN_REBUILD:
        m_eval->OnRebuildConnection();
        dirty = true;
        break;
    case bp::MSG_BP_NODE_PROP_CHANGED:
        dirty = UpdateNodeProp(variants);
        break;
	}

	if (dirty)
    {
        m_sub_mgr->NotifyObservers(MSG_RULE_CHANGED);
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

void WxGraphPage::Traverse(std::function<bool(const ee0::GameObj&)> func,
                           const ee0::VariantSet& variants , bool inverse) const
{
    m_root->GetSharedComp<n0::CompComplex>().Traverse(func, inverse);
}

void WxGraphPage::SetRootNode(const ee0::GameObj& root)
{
    m_root = root;

    //eval->ClearAllNodes();
    //m_eval = std::make_shared<Evaluator>(eval);

    //if (root && root->HasSharedComp<n0::CompComplex>())
    //{
    //    auto& ccomplex = root->GetSharedComp<n0::CompComplex>();
    //    for (auto& c : ccomplex.GetAllChildren())
    //    {
    //        if (c->HasUniqueComp<bp::CompNode>()) {
    //            auto& cnode = c->GetUniqueComp<bp::CompNode>();
    //            auto bp_node = cnode.GetNode();
    //            if (bp_node) {
    //                m_eval->OnAddNode(*bp_node, c);
    //            }
    //        }
    //    }
    //}
    //m_eval->OnRebuildConnection();
}

void WxGraphPage::LoadFromRoot(const ee0::GameObj& root)
{
    if (m_root == root) {
        return;
    }

    ClearAllSceneObjs();

    if (!root) {
        return;
    }

    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& dst = m_root->GetSharedComp<n0::CompComplex>();
    assert(root->HasSharedComp<n0::CompComplex>());
    auto& src = root->GetSharedComp<n0::CompComplex>();
    for (auto& csrc : src.GetAllChildren()) {
        InsertScenNode(dst, csrc);
    }

    for (auto& csrc : src.GetAllChildren())
    {
        if (!csrc->HasUniqueComp<bp::CompNode>()) {
            continue;
        }

        auto& bp_node = csrc->GetUniqueComp<bp::CompNode>().GetNode();
        for (auto& out : bp_node->GetAllOutput()) {
            for (auto& conn : out->GetConnecting()) {
                m_eval->OnConnected(*conn);
            }
        }
    }

    m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

bool WxGraphPage::ClearAllSceneObjs()
{
    // update scene node
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    bool dirty = !ccomplex.GetAllChildren().empty();
    ccomplex.RemoveAllChildren();

    // front eval cb
    assert(m_eval);
    m_eval->OnClearAllNodes();

    return true;
}

bool WxGraphPage::InsertSceneObj(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("obj");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
    GD_ASSERT(obj, "err scene obj");

    // update scene node
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    InsertScenNode(ccomplex, *obj);

    return true;
}

bool WxGraphPage::DeleteSceneObj(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("obj");
	GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
	GD_ASSERT(obj, "err scene obj");

    // update scene (*obj)
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    bool dirty = ccomplex.RemoveChild(*obj);

    if (dirty && (*obj)->HasUniqueComp<bp::CompNode>()) {
        auto& bp_node = (*obj)->GetUniqueComp<bp::CompNode>().GetNode();
        m_eval->OnRemoveNode(*bp_node);
    }

    return dirty;
}

bool WxGraphPage::AfterInsertNodeConn(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("conn");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: conn");
    const std::shared_ptr<bp::Connecting>* conn = static_cast<const std::shared_ptr<bp::Connecting>*>(var.m_val.pv);
    GD_ASSERT(conn, "err conn");

    m_eval->OnConnected(**conn);

    return true;
}

bool WxGraphPage::BeforeDeleteNodeConn(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("conn");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: conn");
    const std::shared_ptr<bp::Connecting>* conn = static_cast<const std::shared_ptr<bp::Connecting>*>(var.m_val.pv);
    GD_ASSERT(conn, "err conn");

    m_eval->OnDisconnecting(**conn);

    return true;
}

bool WxGraphPage::UpdateNodeProp(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("obj");
    if (var.m_type == ee0::VT_EMPTY) {
        return false;
    }

    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
    GD_ASSERT(obj, "err scene obj");

    if ((*obj)->HasUniqueComp<bp::CompNode>())
    {
        auto& bp_node = (*obj)->GetUniqueComp<bp::CompNode>().GetNode();
        m_eval->OnNodePropChanged(bp_node);
        UpdateAABB(*obj);

        return true;
    }
    else
    {
        return false;
    }
}

void WxGraphPage::UpdateAABB(const ee0::GameObj& obj)
{
    auto& bp_node = obj->GetUniqueComp<bp::CompNode>().GetNode();

    auto& st = bp_node->GetStyle();
    obj->GetUniqueComp<n2::CompBoundingBox>().SetSize(
        *obj, sm::rect(st.width, st.height)
    );
}

void WxGraphPage::InsertScenNode(n0::CompComplex& root,
                                 const n0::SceneNodePtr& node)
{
    root.AddChild(node);

    if (!node->HasUniqueComp<bp::CompNode>()) {
        return;
    }

    auto& bp_node = node->GetUniqueComp<bp::CompNode>().GetNode();

    // front eval cb
    m_eval->OnNodeSetup(node);
    m_eval->OnAddNode(*bp_node, node);
    UpdateAABB(node);

    //// update flags
    //auto type = bp_node->get_type();
    //if (type.is_derived_from<Node>()) {
    //    auto sopv_node = std::static_pointer_cast<Node>(bp_node);
    //    if (m_enable_set_node_display) {
    //        sopv_node->SetDisplay(true);
    //    }
    //}
}

}
