#pragma once

#include "archlab/Scene.h"

#include <ee0/Observer.h>
#include <ee0/SubjectMgr.h>
#include <ee0/typedef.h>

#include <archgraph/EvalContext.h>
#include <node0/typedef.h>

#include <wx/panel.h>

namespace ur { class Device; }

class wxNotebook;

namespace archlab
{

class WxTextPage;
class WxGraphPage;

class WxEditorPanel : public wxPanel, public ee0::Observer
{
public:
    WxEditorPanel(const ur::Device& dev, wxWindow* parent, const ee0::SubjectMgrPtr& preview_sub_mgr,
        std::function<WxGraphPage*(wxWindow*, Scene&, archgraph::EvalContext&)> graph_page_creator);
    virtual ~WxEditorPanel();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    void SaveRuleToFile(const std::string& filepath);
    void LoadRuleFromFile(const ur::Device& dev, const std::string& filepath);

    bool IsCurrGraphPage() const;
    n0::SceneNodePtr GetCurrPagePreviewObj() const;

    auto& GetSubMgr() { return m_sub_mgr; }

    auto& GetScene() const { return m_scene; }

    std::shared_ptr<cga::StringPool> GetTextPageStrPool() const;

private:
    void InitLayout(std::function<WxGraphPage*(wxWindow*, Scene&, archgraph::EvalContext&)> graph_page_creator);

    void ShowRule(const std::string& name);

private:
    enum PageIndex
    {
        GRAPH_PAGE_IDX = 0,
        TEXT_PAGE_IDX,
    };

private:
    const ur::Device& m_dev;

    ee0::SubjectMgr m_sub_mgr;
    ee0::SubjectMgrPtr m_preview_sub_mgr = nullptr;

    wxNotebook* m_nb;

    WxGraphPage* m_graph_page;
    WxTextPage*  m_text_page;

    archgraph::EvalContext m_ctx;

    Scene m_scene;

}; // WxEditorPanel

}