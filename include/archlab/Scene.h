#pragma once

#include <rapidjson/document.h>

#include <vector>
#include <memory>

namespace archgraph { class EvalContext; class EvalRule; }
namespace mm { class LinearAllocator; }
namespace n0 { class SceneNode; }
namespace cga { class StringPool; }
namespace ur2 { class Device; }

namespace archlab
{

class Rule;

class Scene
{
public:
    struct Rule
    {
        std::string filepath;
        std::string name;

        std::shared_ptr<archgraph::EvalContext> ctx  = nullptr;
        std::shared_ptr<archgraph::EvalRule>    impl = nullptr;

        std::shared_ptr<n0::SceneNode> root = nullptr;
        std::string text;
    };

public:
    std::shared_ptr<Rule>
        AddRule(const std::string& filepath, const std::shared_ptr<archgraph::EvalRule>& rule);
    std::shared_ptr<Rule> QueryRule(const std::string& filepath) const;

    auto& GetAllRules() const { return m_rules; }

    void StoreToJson(const std::string& dir, rapidjson::Value& val,
        rapidjson::MemoryPoolAllocator<>& alloc) const;
    void LoadFromJson(const ur2::Device& dev, mm::LinearAllocator& alloc, const std::string& dir,
        const rapidjson::Value& val, const std::shared_ptr<cga::StringPool>& str_pool);

private:
    static std::shared_ptr<Rule> CreateRule(const ur2::Device& dev,
        const std::string& filepath,  const std::shared_ptr<cga::StringPool>& str_pool);

private:
    std::vector<std::shared_ptr<Rule>> m_rules;

}; // Scene

}