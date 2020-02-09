#pragma once

#include <blueprint/typedef.h>

#include <ce/EvalOp.h>
#include <ce/typedef.h>
#include <node0/typedef.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>

namespace bp { class Connecting; }

namespace cev
{

class Evaluator : boost::noncopyable
{
public:
    Evaluator();

    // update nodes
    void OnAddNode(const bp::Node& node, const n0::SceneNodePtr& snode,
        bool need_update = true);
    void OnRemoveNode(const bp::Node& node);
    void OnClearAllNodes();

    // update node prop
    void OnNodePropChanged(const bp::NodePtr& node);

    // update node conn
    void OnConnected(const bp::Connecting& conn);
    void OnDisconnecting(const bp::Connecting& conn);
    void OnRebuildConnection();

    // calc
    float CalcFloat(const std::string& expr, const ce::Operation& node, float expect = 0.0f) const;
    int CalcInt(const std::string& expr, const ce::Operation& node, int expect = 0) const;

    auto& GetEvalCtx() const { return m_eval_ctx; }
    auto& GetEval() const { return m_eval; }

    auto& GetAllNodes() const { return m_nodes_map; }

    ce::OpPtr QueryBackNode(const bp::Node& front_node) const;

private:
    void Update();

private:
    std::shared_ptr<ce::EvalContext> m_eval_ctx = nullptr;
    ce::EvalOp m_eval;

    std::unordered_map<const bp::Node*, ce::OpPtr> m_nodes_map;

}; // Evaluator

}