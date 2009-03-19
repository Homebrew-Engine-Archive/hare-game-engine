#include "PCH.h"
#include "outputWindow.h"
#include "LuaDebugger.h"

BEGIN_EVENT_TABLE(LuaOutputWindow, wxPanel)
END_EVENT_TABLE()

LuaOutputWindow::LuaOutputWindow(wxWindow* parent, LuaDebugger* dbg)
: debugger(dbg)
{
    wxXmlResource::Get()->LoadPanel(this, parent, _T("luaLogger"));
    clear();

    wxTextCtrl* txt = XRCCTRL(*this, "txtLuaLogger", wxTextCtrl);
    txt->SetBackgroundColour(*wxWHITE);

    wxFont default_font(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxFont bold_font(default_font);
    bold_font.SetWeight(wxFONTWEIGHT_BOLD);

    type[Log_Print].SetFont(default_font);
    type[Log_Print].SetTextColour(*wxBLACK);

    type[Log_Info].SetFont(default_font);
    type[Log_Info].SetTextColour(*wxBLUE);

    type[Log_Error].SetFont(bold_font);
    type[Log_Error].SetTextColour(*wxRED);
}

LuaOutputWindow::~LuaOutputWindow()
{
}

void LuaOutputWindow::clear()
{
    wxTextCtrl* txt = XRCCTRL(*this, "txtLuaLogger", wxTextCtrl);
    txt->Clear();
}

void LuaOutputWindow::append(const wxString& msg, LuaLoggerType tp)
{
    if (msg.IsEmpty())
        return;

    wxTextCtrl* txt = XRCCTRL(*this, "txtLuaLogger", wxTextCtrl);

    txt->SetDefaultStyle(type[tp]);
    txt->AppendText(msg);
};

bool LuaOutputWindow::isReallyShown()
{
    wxWindow* win = this;
    while (win && win->IsShown())
    {
        win = win->GetParent();
        if (!win)
            return true;
    }
    return false;
}

