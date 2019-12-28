#pragma once

#include <wx/panel.h>

namespace cgav
{

class WxTextPage;
class WxGraphPage;

class WxEditorPanel : public wxPanel
{
public:
    WxEditorPanel(wxWindow* parent, std::function<WxGraphPage*(wxWindow*)> graph_page_creator);

    auto GetGraphPage() const { return m_graph_page; }

private:
    void InitLayout(std::function<WxGraphPage*(wxWindow*)> graph_page_creator);

private:
    WxTextPage*  m_text_page;
    WxGraphPage* m_graph_page;

}; // WxEditorPanel

}