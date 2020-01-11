#pragma once

#include <rapidjson/document.h>

#include <vector>
#include <memory>

namespace cga { class EvalRule; }
namespace mm { class LinearAllocator; }
namespace n0 { class SceneNode; }

namespace cgav
{

class Rule;

class Scene
{
public:
    struct Rule
    {
        std::string filepath;
        std::string name;
        std::shared_ptr<cga::EvalRule> impl = nullptr;

        std::shared_ptr<n0::SceneNode> root = nullptr;
        std::string text;
    };

public:
    std::shared_ptr<Rule>
        AddRule(const std::string& filepath, const std::shared_ptr<cga::EvalRule>& rule);
    bool ChangeRule(const std::string& filepath, const std::shared_ptr<cga::EvalRule>& rule);

    auto& GetAllRules() const { return m_rules; }

    void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const;
    void LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val);

private:
    static std::shared_ptr<Rule> CreateRule(const std::string& filepath);

private:
    std::vector<std::shared_ptr<Rule>> m_rules;

}; // Scene

}