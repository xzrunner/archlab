#include "archlab/Scene.h"
#include "archlab/Evaluator.h"

#include <blueprint/CompNode.h>
#include <blueprint/NSCompNode.h>
#include <blueprint/Node.h>
#include <blueprint/Pin.h>

#include <ns/NodeFactory.h>
#include <sx/ResFileHelper.h>
#include <js/RapidJsonHelper.h>
#include <archgraph/RuleLoader.h>
#include <archgraph/EvalRule.h>
#include <node0/SceneNode.h>
#include <node0/CompComplex.h>

#include <boost/filesystem.hpp>

namespace archlab
{

std::shared_ptr<Scene::Rule>
Scene::AddRule(const std::string& filepath, const std::shared_ptr<archgraph::EvalRule>& rule)
{
    auto find = QueryRule(filepath);
    if (find)
    {
        find->impl = rule;
        return find;
    }
    else
    {
        auto r = std::make_shared<Rule>();

        r->filepath = filepath;
        r->name = boost::filesystem::path(filepath).filename().string();
        r->impl = rule;
        m_rules.push_back(r);

        return r;
    }
}

std::shared_ptr<Scene::Rule>
Scene::QueryRule(const std::string& filepath) const
{
    for (auto& rule : m_rules) {
        if (rule->filepath == filepath) {
            return rule;
        }
    }
    return nullptr;
}

void Scene::StoreToJson(const std::string& dir, rapidjson::Value& val,
                        rapidjson::MemoryPoolAllocator<>& alloc) const
{
    val.SetArray();
    for (auto& rule : m_rules)
    {
        auto relative = boost::filesystem::relative(rule->filepath, dir).string();
        val.PushBack(rapidjson::Value(relative.c_str(), alloc), alloc);
    }
}

void Scene::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir,
                         const rapidjson::Value& val, const std::shared_ptr<cga::StringPool>& str_pool)
{
    m_rules.clear();

    for (auto& path_val : val.GetArray())
    {
        auto absolute = boost::filesystem::absolute(path_val.GetString(), dir).string();
        auto rule = CreateRule(dev, absolute, str_pool);
        assert(rule);
        m_rules.push_back(rule);
    }
}

std::shared_ptr<Scene::Rule>
Scene::CreateRule(const ur::Device& dev, const std::string& filepath, const std::shared_ptr<cga::StringPool>& str_pool)
{
    std::shared_ptr<Rule> rule = std::make_shared<Rule>();
    rule->filepath = filepath;
    rule->name = boost::filesystem::path(filepath).filename().string();

    auto type = sx::ResFileHelper::Type(filepath);
    if (type == sx::RES_FILE_JSON)
    {
        assert(sx::ResFileHelper::Type(filepath) == sx::RES_FILE_JSON);
        rapidjson::Document doc;
        js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

        auto dir = boost::filesystem::path(filepath).parent_path().string();
        auto node = ns::NodeFactory::Create(dev, filepath);

        Evaluator eval(dev);
        assert(node->HasSharedComp<n0::CompComplex>());
        auto& ccomplex = node->GetSharedComp<n0::CompComplex>();
        for (auto& cnode : ccomplex.GetAllChildren())
        {
            if (!cnode->HasUniqueComp<bp::CompNode>()) {
                continue;
            }

            auto& bp_node = cnode->GetUniqueComp<bp::CompNode>().GetNode();
            eval.OnAddNode(*bp_node, cnode);
        }

        // setup conns
        bp::NSCompNode::LoadConnection(ccomplex.GetAllChildren(), doc["nodes"]);
        for (auto& cnode : ccomplex.GetAllChildren())
        {
            if (!cnode->HasUniqueComp<bp::CompNode>()) {
                continue;
            }

            auto& bp_node = cnode->GetUniqueComp<bp::CompNode>().GetNode();
            for (auto& out : bp_node->GetAllOutput()) {
                for (auto& conn : out->GetConnecting()) {
                    eval.OnConnected(*conn);
                }
            }
        }

        rule->impl = eval.GetEval().ToRule(*eval.GetEvalCtx());

        rule->root = node;
    }
    else
    {
        auto ext = boost::filesystem::extension(filepath);
        std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
        assert(ext == ".archgraph");

        std::ifstream fin(filepath);
        std::string str((std::istreambuf_iterator<char>(fin)),
            std::istreambuf_iterator<char>());
        fin.close();

        archgraph::RuleLoader loader(str_pool);
        auto eval_ctx = std::make_shared<archgraph::EvalContext>();
        auto eval = std::make_shared<archgraph::EvalRule>();
        loader.RunString(*eval_ctx, str, *eval/*, true*/);

        rule->impl = eval;

        rule->text = str;
    }

    return rule;
}

}