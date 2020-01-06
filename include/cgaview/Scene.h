#pragma once

#include <rapidjson/document.h>

#include <vector>
#include <memory>

namespace cga { class EvalRule; }
namespace mm { class LinearAllocator; }

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
    };

public:
    void AddRule(const std::string& filepath, const std::shared_ptr<cga::EvalRule>& rule);

    auto& GetAllRules() const { return m_rules; }

    void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const;
    void LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val);

    static std::shared_ptr<cga::EvalRule> CreateRule(const std::string& filepath);

private:
    std::vector<std::shared_ptr<Rule>> m_rules;

}; // Scene

}