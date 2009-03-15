#include "PCH.h"
#include "CallStackWindow.h"
#include "LuaDebuggerPlugin.h"

BEGIN_EVENT_TABLE(LuaCallStackWindow, wxPanel)
    EVT_LIST_ITEM_ACTIVATED(XRCID("lstLuaCallStack"), LuaCallStackWindow::onDbClick)
END_EVENT_TABLE()

LuaCallStackWindow::LuaCallStackWindow(wxWindow* parent, LuaDebugger* dbg)
: debugger(dbg)
{
    wxXmlResource::Get()->LoadPanel(this, parent, _T("luaCallStack"));
    clear();
}

LuaCallStackWindow::~LuaCallStackWindow()
{
}

void LuaCallStackWindow::clear()
{
    wxListCtrl* lst = XRCCTRL(*this, "lstLuaCallStack", wxListCtrl);
    lst->ClearAll();
    lst->Freeze();
    lst->DeleteAllItems();
    lst->InsertColumn(0, wxT(" "), wxLIST_FORMAT_RIGHT, 20);
    lst->InsertColumn(1, _("Name"), wxLIST_FORMAT_LEFT, 200);
    lst->Thaw();
}

void LuaCallStackWindow::setCallStackData(LuaDebugData* data)
{
    callStackData = data;
    clear();

    if (!callStackData)
        return;

    wxListCtrl* lst = XRCCTRL(*this, "lstLuaCallStack", wxListCtrl);

    lst->Freeze();
    for (size_t i = 0; i < callStackData->items.size(); ++i)
    {
        int idx = lst->InsertItem(i, 0);
        wxString name = wxString::FromUTF8(callStackData->items[i]->itemKey.c_str());
        lst->SetItem(idx, 1, name);
    }
    lst->Thaw();
}

void LuaCallStackWindow::onDbClick(wxListEvent& event)
{
    if (!callStackData)
        return;

    wxListCtrl* lst = XRCCTRL(*this, "lstLuaCallStack", wxListCtrl);
    if (lst->GetSelectedItemCount() == 0)
        return;

    int index = lst->GetNextItem(-1,
        wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED);

    if (index >=0 && index < (int)callStackData->items.size())
    {
        String fileName = callStackData->items[index]->itemSource;
        String fileLine = callStackData->items[index]->itemValue;
        if (!fileName.empty() && !fileLine.empty())
        {
            int lineNumber = 0;
            StringConverter::parse(fileLine, lineNumber);
            debugger->syncEditor(fileName, lineNumber);
        }
    }
}

bool LuaCallStackWindow::isReallyShown()
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
