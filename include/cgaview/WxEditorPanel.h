#pragma once

#include "cgaview/Scene.h"

#include <ee0/SubjectMgr.h>

#include <cga/EvalContext.h>

#include <wx/panel.h>

class wxNotebook;

namespace cgav
{

class WxTextPage;
class WxGraphPage;

class WxEditorPanel : public wxPanel
{
public:
    WxEditorPanel(wxWindow* parent,
        std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator);

    void SaveRuleToFile(const std::string& filepath);
    void LoadRuleFromFile(const std::string& filepath);

    bool IsCurrGraphPage() const;

    auto& GetSubMgr() { return m_sub_mgr; }

    auto& GetScene() const { return m_scene; }

private:
    void InitLayout(std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator);

private:
    enum PageIndex
    {
        GRAPH_PAGE_IDX = 0,
        TEXT_PAGE_IDX,
    };

private:
    ee0::SubjectMgr m_sub_mgr;

    wxNotebook* m_nb;

    WxGraphPage* m_graph_page;
    WxTextPage*  m_text_page;

    cga::EvalContext m_ctx;

    Scene m_scene;

}; // WxEditorPanel

}