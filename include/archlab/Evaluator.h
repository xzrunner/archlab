#pragma once

#include <blueprint/typedef.h>

#include <archgraph/EvalOp.h>
#include <archgraph/typedef.h>
#include <node0/typedef.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>

namespace bp { class Connecting; }

namespace archlab
{

class Evaluator : boost::noncopyable
{
public:
    Evaluator(const ur2::Device& dev);

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
    float CalcFloat(const std::string& expr, const archgraph::Operation& node, float expect = 0.0f) const;
    int CalcInt(const std::string& expr, const archgraph::Operation& node, int expect = 0) const;

    auto& GetEvalCtx() const { return m_eval_ctx; }
    auto& GetEval() const { return m_eval; }

    auto& GetAllNodes() const { return m_nodes_map; }

    archgraph::OpPtr QueryBackNode(const bp::Node& front_node) const;

private:
    void Update();

private:
    const ur2::Device& m_dev;

    std::shared_ptr<archgraph::EvalContext> m_eval_ctx = nullptr;
    archgraph::EvalOp m_eval;

    std::unordered_map<const bp::Node*, archgraph::OpPtr> m_nodes_map;

}; // Evaluator

}