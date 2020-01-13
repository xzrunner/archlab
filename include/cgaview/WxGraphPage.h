#pragma once

#include <ee0/WxStagePage.h>

namespace n0 { class CompComplex; }

namespace cgav
{

class Evaluator;
class Scene;

class WxGraphPage : public ee0::WxStagePage
{
public:
	WxGraphPage(wxWindow* parent, Scene& scene,
        const ee0::SubjectMgrPtr& preview_sub_mgr, const ee0::GameObj& root);
    virtual ~WxGraphPage();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    virtual void Traverse(std::function<bool(const ee0::GameObj&)> func,
        const ee0::VariantSet& variants = ee0::VariantSet(), bool inverse = false) const override;

    auto GetEval() const { return m_eval; }

    auto GetRootNode() const { return m_root; }

    void LoadFromRoot(const ee0::GameObj& root);

    void SetRulePath(const std::string& path) { m_rule_path = path; }
    auto& GetRulePath() const { return m_rule_path; }

    auto GetPreviewObj() const { return m_preview_obj; }

private:
    bool ClearAllSceneObjs();
    bool InsertSceneObj(const ee0::VariantSet& variants);
    bool DeleteSceneObj(const ee0::VariantSet& variants);

    bool AfterInsertNodeConn(const ee0::VariantSet& variants);
    bool BeforeDeleteNodeConn(const ee0::VariantSet& variants);
    bool UpdateNodeProp(const ee0::VariantSet& variants);

    void UpdateAABB(const ee0::GameObj& obj);

    void InsertScenNode(n0::CompComplex& root,
        const n0::SceneNodePtr& node);

private:
    Scene& m_scene;

    ee0::SubjectMgrPtr m_preview_sub_mgr = nullptr;

    n0::SceneNodePtr m_root = nullptr;

    std::shared_ptr<Evaluator> m_eval = nullptr;

    std::string m_rule_path = "default_graph_rule";

    n0::SceneNodePtr m_preview_obj = nullptr;

}; // WxGraphPage

}