#pragma once

#include <cga/EvalContext.h>

#include <wx/panel.h>

namespace cgav
{

class WxTextPage;
class WxGraphPage;

class WxEditorPanel : public wxPanel
{
public:
    WxEditorPanel(wxWindow* parent,
        std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator);

    auto GetGraphPage() const { return m_graph_page; }

private:
    void InitLayout(std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator);

private:
    WxTextPage*  m_text_page;
    WxGraphPage* m_graph_page;

    cga::EvalContext m_ctx;

}; // WxEditorPanel

}