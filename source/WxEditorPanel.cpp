#include "cgaview/WxEditorPanel.h"
#include "cgaview/WxTextPage.h"
#include "cgaview/WxGraphPage.h"

#include <wx/notebook.h>
#include <wx/sizer.h>

namespace cgav
{

WxEditorPanel::WxEditorPanel(wxWindow* parent, std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator)
    : wxPanel(parent)
{
    InitLayout(graph_page_creator);
}

void WxEditorPanel::InitLayout(std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);

    // property
    auto prop = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
    prop->AddPage(m_text_page = new WxTextPage(prop), "Text");
    prop->AddPage(m_graph_page = graph_page_creator(prop, m_ctx), "Graph");
    sizer->Add(prop, 1, wxEXPAND);

    SetSizer(sizer);
}

}