#include "archlab/WxEditorPanel.h"
#include "archlab/WxTextPage.h"
#include "archlab/WxGraphPage.h"
#include "archlab/Evaluator.h"
#include "archlab/MessageID.h"
#include "archlab/ModelAdapter.h"

#include <blueprint/NSCompNode.h>
#include <blueprint/Serializer.h>

#include <js/RapidJsonHelper.h>
#include <sx/ResFileHelper.h>
#include <node0/SceneNode.h>
#include <node0/CompComplex.h>
#include <easyarchgraph/CompArchGraph.h>

#include <wx/notebook.h>
#include <wx/sizer.h>

#include <boost/filesystem.hpp>

namespace archlab
{

WxEditorPanel::WxEditorPanel(const ur2::Device& dev, wxWindow* parent, const ee0::SubjectMgrPtr& preview_sub_mgr,
                             std::function<WxGraphPage*(wxWindow*, Scene&, archgraph::EvalContext&)> graph_page_creator)
    : wxPanel(parent)
    , m_dev(dev)
    , m_preview_sub_mgr(preview_sub_mgr)
{
    InitLayout(graph_page_creator);

    m_sub_mgr.RegisterObserver(MSG_RULE_SHOW, this);
    m_preview_sub_mgr->RegisterObserver(MSG_RULE_CHANGED, this);
}

WxEditorPanel::~WxEditorPanel()
{
    m_sub_mgr.UnregisterObserver(MSG_RULE_SHOW, this);
    m_preview_sub_mgr->UnregisterObserver(MSG_RULE_CHANGED, this);
}

void WxEditorPanel::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    switch (msg)
    {
    case MSG_RULE_SHOW:
    {
        auto var = variants.GetVariant("name");
        assert(var.m_type == ee0::VT_PCHAR);
        ShowRule(var.m_val.pc);
    }
        break;
    case MSG_RULE_CHANGED:
    {
        auto var_rule = variants.GetVariant("rule");
        GD_ASSERT(var_rule.m_type == ee0::VT_PVOID, "err var");
        const std::shared_ptr<archgraph::EvalRule>* rule
            = static_cast<const std::shared_ptr<archgraph::EvalRule>*>(var_rule.m_val.pv);

        auto var_ctx = variants.GetVariant("ctx");
        GD_ASSERT(var_ctx.m_type == ee0::VT_PVOID, "err var");
        const std::shared_ptr<archgraph::EvalContext>* rule_ctx
            = static_cast<const std::shared_ptr<archgraph::EvalContext>*>(var_ctx.m_val.pv);

        auto node = GetCurrPagePreviewObj();
        auto& ccga = node->GetUniqueComp<easyarchgraph::CompArchGraph>();
        if (ccga.GetRule() != *rule) {
            ccga.SetRule(*rule, *rule_ctx);
        }
        ModelAdapter::BuildModel(m_dev, *node);
    }
        break;
    }
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

        auto root = m_graph_page->GetRootNode();
        bp::Serializer::StoreToJson(root, dir, doc, alloc);
        assert(root->HasSharedComp<n0::CompComplex>());
        auto& ccomplex = root->GetSharedComp<n0::CompComplex>();
        bp::NSCompNode::StoreConnection(ccomplex.GetAllChildren(), doc["nodes"], alloc);

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

void WxEditorPanel::LoadRuleFromFile(const ur2::Device& dev, const std::string& filepath)
{
    for (auto& rule : m_scene.GetAllRules()) {
        if (rule->filepath == filepath) {
            return;
        }
    }

    auto filename = boost::filesystem::path(filepath).filename();
    std::shared_ptr<archgraph::EvalRule> rule = nullptr;

    switch (m_nb->GetSelection())
    {
    case GRAPH_PAGE_IDX:
    {
        assert(sx::ResFileHelper::Type(filepath) == sx::RES_FILE_JSON);

        m_graph_page->SetRulePath(filepath);

        rapidjson::Document doc;
        js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

        auto dir = boost::filesystem::path(filepath).parent_path().string();
        auto root = m_graph_page->GetRootNode();
        bp::Serializer::LoadFromJson(dev, *m_graph_page, root, doc, dir);

        assert(root->HasSharedComp<n0::CompComplex>());
        auto& ccomplex = root->GetSharedComp<n0::CompComplex>();
        bp::NSCompNode::LoadConnection(ccomplex.GetAllChildren(), doc["nodes"]);

        auto eval = m_graph_page->GetEval();
        rule = eval->GetEval().ToRule(*eval->GetEvalCtx());
    }
        break;
    case TEXT_PAGE_IDX:
    {
        std::ifstream fin(filepath);
        std::string str((std::istreambuf_iterator<char>(fin)),
            std::istreambuf_iterator<char>());
        fin.close();

        m_text_page->SetRulePath(filepath);

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

n0::SceneNodePtr
WxEditorPanel::GetCurrPagePreviewObj() const
{
    auto id = m_nb->GetSelection();
    assert(id == 0 || id == 1);
    n0::SceneNodePtr node = nullptr;
    if (id == 0) {
        node = m_graph_page->GetPreviewObj();
    } else {
        node = m_text_page->GetPreviewObj();
    }
    assert(node);
    return node;
}

std::shared_ptr<cga::StringPool>
WxEditorPanel::GetTextPageStrPool() const
{
    return m_text_page->GetStringPool();
}

void WxEditorPanel::InitLayout(std::function<WxGraphPage*(wxWindow*, Scene&, archgraph::EvalContext&)> graph_page_creator)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);

    // property
    m_nb = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
    m_nb->AddPage(m_graph_page = graph_page_creator(m_nb, m_scene, m_ctx), "Graph");
    m_nb->AddPage(m_text_page = new WxTextPage(m_dev, m_nb, m_scene, m_preview_sub_mgr), "Text");
    sizer->Add(m_nb, 1, wxEXPAND);

    SetSizer(sizer);
}

void WxEditorPanel::ShowRule(const std::string& name)
{
    for (auto& rule : m_scene.GetAllRules())
    {
        if (rule->name != name) {
            continue;
        }

        if (rule->root)
        {
            m_text_page->SetText("");

            m_graph_page->LoadFromRoot(rule->root);
        }
        else
        {
            assert(!rule->text.empty());
            m_text_page->SetText(rule->text);

            m_graph_page->LoadFromRoot(nullptr);
        }
    }
}

}