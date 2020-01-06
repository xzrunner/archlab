#include "cgaview/WxEditorPanel.h"
#include "cgaview/WxTextPage.h"
#include "cgaview/WxGraphPage.h"
#include "cgaview/Serializer.h"
#include "cgaview/Evaluator.h"
#include "cgaview/MessageID.h"

#include <js/RapidJsonHelper.h>
#include <sx/ResFileHelper.h>

#include <wx/notebook.h>
#include <wx/sizer.h>

#include <boost/filesystem.hpp>

namespace cgav
{

WxEditorPanel::WxEditorPanel(wxWindow* parent, std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator)
    : wxPanel(parent)
{
    InitLayout(graph_page_creator);
}

void WxEditorPanel::SaveRuleToFile(const std::string& filepath)
{
    switch (m_nb->GetSelection())
    {
    case GRAPH_PAGE_IDX:
    {
        rapidjson::Document doc;
        doc.SetObject();

        auto dir = boost::filesystem::path(filepath).parent_path().string();
        auto& alloc = doc.GetAllocator();

        Serializer::StoreToJson(m_graph_page->GetRootNode(), dir, doc, alloc);

        js::RapidJsonHelper::WriteToFile(filepath.c_str(), doc);
    }
        break;
    case TEXT_PAGE_IDX:
    {
        std::locale::global(std::locale(""));
        std::ofstream fout(filepath.c_str());
        std::locale::global(std::locale("C"));
        fout << m_text_page->GetText();
        fout.close();
    }
        break;
    default:
        assert(0);
    }
}

void WxEditorPanel::LoadRuleFromFile(const std::string& filepath)
{
    auto filename = boost::filesystem::path(filepath).filename();
    std::shared_ptr<cga::EvalRule> rule = nullptr;

    switch (m_nb->GetSelection())
    {
    case GRAPH_PAGE_IDX:
    {
        assert(sx::ResFileHelper::Type(filepath) == sx::RES_FILE_JSON);
        rapidjson::Document doc;
        js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

        auto dir = boost::filesystem::path(filepath).parent_path().string();
        cgav::Serializer::LoadFromJson(*m_graph_page, m_graph_page->GetRootNode(), doc, dir);

        rule = m_graph_page->GetEval()->GetEval().ToRule();
    }
        break;
    case TEXT_PAGE_IDX:
    {
        std::ifstream fin(filepath);
        std::string str((std::istreambuf_iterator<char>(fin)),
            std::istreambuf_iterator<char>());
        fin.close();
        m_text_page->SetText(str);

        rule = m_text_page->GetEval();
    }
        break;
    default:
        assert(0);
    }

    if (rule)
    {
        m_scene.AddRule(filepath, rule);

        ee0::VariantSet vars;

        auto name_str = filename.string();

        ee0::Variant var_name;
        var_name.m_type = ee0::VT_PCHAR;
        char* tmp = new char[name_str.size() + 1];
        strcpy(tmp, name_str.c_str());
        var_name.m_val.pc = tmp;
        vars.SetVariant("name", var_name);

        m_sub_mgr.NotifyObservers(MSG_RULE_INSERTED, vars);
    }
}

bool WxEditorPanel::IsCurrGraphPage() const
{
    return m_nb->GetSelection() == 0;
}

void WxEditorPanel::InitLayout(std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);

    // property
    m_nb = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
    m_nb->AddPage(m_graph_page = graph_page_creator(m_nb, m_ctx), "Graph");
    m_nb->AddPage(m_text_page = new WxTextPage(m_nb), "Text");
    sizer->Add(m_nb, 1, wxEXPAND);

    SetSizer(sizer);
}

}