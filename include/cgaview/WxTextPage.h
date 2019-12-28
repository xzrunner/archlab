#pragma once

#include <wx/panel.h>

namespace ee0 { class WxCodeCtrl; }

namespace cgav
{

class WxTextPage : public wxPanel
{
public:
    WxTextPage(wxWindow* parent);

private:
    void InitLayout();

    void OnRunPress(wxCommandEvent& event);

private:
    ee0::WxCodeCtrl* m_code;

}; // WxTextPage

}