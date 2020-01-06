#include "cgaview/Scene.h"
#include "cgaview/Serializer.h"
#include "cgaview/Evaluator.h"

#include <blueprint/CompNode.h>
#include <blueprint/NSCompNode.h>

#include <ns/NodeFactory.h>
#include <sx/ResFileHelper.h>
#include <js/RapidJsonHelper.h>
#include <cga/RuleLoader.h>
#include <cga/EvalRule.h>
#include <node0/SceneNode.h>
#include <node0/CompComplex.h>

#include <boost/filesystem.hpp>

namespace cgav
{

void Scene::AddRule(const std::string& filepath, const std::shared_ptr<cga::EvalRule>& rule)
{
    auto r = std::make_shared<Rule>();
    r->filepath = filepath;
    r->name = boost::filesystem::path(filepath).filename().string();
    r->impl = rule;
    m_rules.push_back(r);
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

void Scene::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir,
                         const rapidjson::Value& val)
{
    m_rules.clear();

    for (auto& path_val : val.GetArray())
    {
        auto absolute = boost::filesystem::absolute(path_val.GetString(), dir).string();
        auto rule = CreateRule(absolute);
        assert(rule);
        AddRule(absolute, rule);
    }
}

std::shared_ptr<cga::EvalRule> Scene::CreateRule(const std::string& filepath)
{
    auto type = sx::ResFileHelper::Type(filepath);
    if (type == sx::RES_FILE_JSON)
    {
        assert(sx::ResFileHelper::Type(filepath) == sx::RES_FILE_JSON);
        rapidjson::Document doc;
        js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

        auto dir = boost::filesystem::path(filepath).parent_path().string();
        auto node = ns::NodeFactory::Create(filepath);

        Evaluator eval;
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

        bp::NSCompNode::LoadConnection(ccomplex.GetAllChildren(), doc["nodes"]);
        // todo: on connected

        return eval.GetEval().ToRule();
    }
    else
    {
        auto ext = boost::filesystem::extension(filepath);
        std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
        assert(ext == ".cga");

        std::ifstream fin(filepath);
        std::string str((std::istreambuf_iterator<char>(fin)),
            std::istreambuf_iterator<char>());
        fin.close();

        cga::RuleLoader loader;
        auto eval = std::make_shared<cga::EvalRule>();
        loader.RunString(str, *eval/*, true*/);

        return eval;
    }
}

}