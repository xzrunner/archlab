#pragma once

#include <blueprint/typedef.h>

#include <cga/Evaluator.h>
#include <cga/typedef.h>
#include <node0/typedef.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>

namespace bp { class Connecting; }

namespace cgav
{

class Evaluator : boost::noncopyable
{
public:
    Evaluator();

    // init
    void OnNodeSetup(const n0::SceneNodePtr& node);

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
    float CalcFloat(const std::string& expr, const cga::Node& node, float expect = 0.0f) const;
    int CalcInt(const std::string& expr, const cga::Node& node, int expect = 0) const;

    auto& GetAllNodes() const { return m_nodes_map; }

    cga::NodePtr QueryBackNode(const bp::Node& front_node) const;

private:
    void Update();

private:
    cga::Evaluator m_eval;

    std::unordered_map<const bp::Node*, cga::NodePtr> m_nodes_map;

}; // Evaluator

}