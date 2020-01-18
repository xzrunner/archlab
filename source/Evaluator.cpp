#include "cgaview/Evaluator.h"
#include "cgaview/CGAAdapter.h"
#include "cgaview/RegistNodes.h"
#include "cgaview/ModelAdapter.h"

#include <blueprint/Node.h>
#include <blueprint/Pin.h>
#include <blueprint/Connecting.h>
#include <blueprint/CompNode.h>

#include <node0/SceneNode.h>
#include <node0/CompComplex.h>
#include <node3/CompModel.h>

#include <queue>

namespace
{

void update_model(const std::vector<cga::GeoPtr>& geos, void* ud)
{
    auto snode = static_cast<n0::SceneNode*>(ud);
    if (snode) {
        if (!snode->HasSharedComp<n3::CompModel>()) {
            cgav::ModelAdapter::SetupModel(*snode);
        }
        cgav::ModelAdapter::UpdateModel(geos, *snode);
    }
}

}

namespace cgav
{

Evaluator::Evaluator()
    : m_eval_ctx(std::make_shared<cga::EvalContext>())
    , m_eval(update_model)
{
}

void Evaluator::OnAddNode(const bp::Node& front, const n0::SceneNodePtr& snode, bool need_update)
{
    auto back = CGAAdapter::CreateBackFromFront(front);
    if (!back) {
        return;
    }

    m_eval.AddOp(back, snode.get());
    m_nodes_map.insert({ &front, back });

    if (front.get_type().is_derived_from<Node>()) {
        const_cast<Node&>(static_cast<const Node&>(front)).SetName(back->GetName());
    }

    CGAAdapter::UpdatePropBackFromFront(front, *back, *this);
    if (need_update) {
        Update();
    }
}

void Evaluator::OnRemoveNode(const bp::Node& node)
{
    auto itr = m_nodes_map.find(&node);
    if (itr == m_nodes_map.end()) {
        return;
    }

    m_eval.RemoveOp(itr->second);
    m_nodes_map.erase(itr);

    Update();
}

void Evaluator::OnClearAllNodes()
{
    m_eval.ClearAllOps();
    m_nodes_map.clear();

    Update();
}

void Evaluator::OnNodePropChanged(const bp::NodePtr& node)
{
    auto itr = m_nodes_map.find(node.get());
    // not cga node
    if (itr == m_nodes_map.end()) {
        return;
    }

    CGAAdapter::UpdatePropBackFromFront(*node, *itr->second, *this);

    if (node->get_type().is_derived_from<Node>())
    {
        auto& cgav_n = std::static_pointer_cast<Node>(node);
        if (cgav_n->GetName() != itr->second->GetName()) {
            m_eval.Rename(itr->second->GetName(), cgav_n->GetName());
            if (itr->second->GetName() != cgav_n->GetName()) {
                cgav_n->SetName(itr->second->GetName());
            }
        }
    }

    //m_eval.PropChanged(itr->second);
    m_eval.MakeDirty();

    Update();
}

void Evaluator::OnConnected(const bp::Connecting& conn)
{
    auto f_pin = conn.GetFrom();
    auto t_pin = conn.GetTo();

    auto f_itr = m_nodes_map.find(&f_pin->GetParent());
    auto t_itr = m_nodes_map.find(&t_pin->GetParent());
    if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
        return;
    }

    if (t_itr->first->GetAllInput().size() > t_itr->second->GetImports().size()) {
        t_itr->second->AddInputPorts(t_itr->first->GetAllInput().size() - t_itr->first->GetAllOutput().size());
    }

    m_eval.Connect(
        { f_itr->second, f_pin->GetPosIdx() },
        { t_itr->second, t_pin->GetPosIdx() }
    );

    Update();
}

void Evaluator::OnDisconnecting(const bp::Connecting& conn)
{
    auto f_pin = conn.GetFrom();
    auto t_pin = conn.GetTo();

    auto f_itr = m_nodes_map.find(&f_pin->GetParent());
    auto t_itr = m_nodes_map.find(&t_pin->GetParent());
    if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
        return;
    }

    m_eval.Disconnect(
        { f_itr->second, f_pin->GetPosIdx() },
        { t_itr->second, t_pin->GetPosIdx() }
    );

    Update();
}

void Evaluator::OnRebuildConnection()
{
    std::vector<std::pair<cga::Operation::PortAddr, cga::Operation::PortAddr>> conns;
    for (auto& itr : m_nodes_map)
    {
        auto& front = itr.first;
        auto& back  = itr.second;
        for (auto& in : front->GetAllInput())
        {
            for (auto& conn : in->GetConnecting())
            {
                auto f_pin = conn->GetFrom();
                auto t_pin = conn->GetTo();

                auto f_itr = m_nodes_map.find(&f_pin->GetParent());
                auto t_itr = m_nodes_map.find(&t_pin->GetParent());
                if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
                    continue;
                }

                if (t_itr->first->GetAllInput().size() > t_itr->second->GetImports().size()) {
                    t_itr->second->AddInputPorts(t_itr->first->GetAllInput().size() - t_itr->first->GetAllOutput().size());
                }

                conns.push_back({
                    { f_itr->second, f_pin->GetPosIdx() },
                    { t_itr->second, t_pin->GetPosIdx() }
                });
            }
        }
    }

    m_eval.RebuildConnections(conns);

    Update();
}

cga::OpPtr Evaluator::QueryBackNode(const bp::Node& front_node) const
{
    auto itr = m_nodes_map.find(&front_node);
    return itr == m_nodes_map.end() ? nullptr : itr->second;
}

void Evaluator::Update()
{
    m_eval.Eval(*m_eval_ctx);

    bool conn_changed = false;
    for (auto& itr : m_nodes_map)
    {
        if (!itr.second->IsPortChanged()) {
            continue;
        }

        if (itr.first->get_type().is_derived_from<Node>()) {
            auto front = static_cast<const Node*>(itr.first);
            const_cast<Node*>(front)->UpdatePins(*itr.second);
            conn_changed = true;
        }
        itr.second->SetPortChanged(false);
    }
    if (conn_changed) {
        OnRebuildConnection();
    }
}

}