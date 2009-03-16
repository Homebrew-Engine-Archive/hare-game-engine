#include "PCH.h"
#include "WatchWindow.h"
#include "LuaDebugger.h"
#include <wx/treelistctrl.h>

static const wxChar* LUA_TYPE_NAME[] = {
    wxT("nil"), wxT("boolean"), wxT("lightuserdata"), wxT("number"), wxT("string"),
    wxT("table"), wxT("function"), wxT("userdata"), wxT("thread")
};

BEGIN_EVENT_TABLE(LuaWatchWindow, wxPanel)
    EVT_TREE_KEY_DOWN(wxID_ANY, onTreeKeyDown)
    EVT_TREE_END_LABEL_EDIT(wxID_ANY, onTreeEndLabelEdit)
END_EVENT_TABLE()

LuaWatchWindow::LuaWatchWindow(wxWindow* parent, LuaDebugger* dbg)
: wxPanel(parent), debugger(dbg)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    treeList = new wxTreeListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
        wxTR_HIDE_ROOT | wxTR_EDIT_LABELS | wxTR_FULL_ROW_HIGHLIGHT | wxTR_ROW_LINES | wxTR_COLUMN_LINES);
    sizer->Add(treeList, 1, wxALIGN_LEFT | wxALIGN_TOP | wxEXPAND, 0);
    SetSizer(sizer);
    Layout();
    sizer->Fit(this);

    treeList->AddColumn(_("Name"), DEFAULT_COL_WIDTH, wxALIGN_LEFT, -1, true, true);
    treeList->AddColumn(_("Value"), 200);
    treeList->AddColumn(_("Type"));

    root = treeList->AddRoot(wxEmptyString);

    addEmptyRow();
}

LuaWatchWindow::~LuaWatchWindow()
{
}

void LuaWatchWindow::addEmptyRow()
{
    treeList->AppendItem(root, wxEmptyString);
}

void LuaWatchWindow::onTreeKeyDown(wxTreeEvent& event)
{
    if (event.GetKeyCode() == WXK_F2)
    {
        treeList->EditLabel(treeList->GetSelection(), 0);
    }
    else if (event.GetKeyCode() == WXK_DELETE)
    {
        wxTreeItemIdValue cookie;
        if (treeList->GetSelection() != treeList->GetLastChild(root, cookie))
        {
            treeList->Delete(treeList->GetSelection());
            event.Veto();
            return;
        }
    }

    event.Skip();
}

void LuaWatchWindow::onTreeEndLabelEdit(wxTreeEvent& event)
{
    if (event.IsEditCancelled())
    {
        event.Skip();
        return;
    }

    wxTreeItemIdValue cookie;
    if (treeList->GetSelection() == treeList->GetLastChild(root, cookie))
    {
        if (event.GetLabel().IsEmpty())
        {
            event.Skip();
            return;
        }

        addEmptyRow();

        debugger->evaluateExpr(debugger->getCurrStackLevel(), event.GetLabel().ToUTF8().data());
    }
    else
    {
        if (event.GetLabel().IsEmpty())
        {
            treeList->Delete(treeList->GetSelection());
            event.Veto();
            return;
        }
        else
        {
            debugger->evaluateExpr(debugger->getCurrStackLevel(), event.GetLabel().ToUTF8().data());
        }
    }
}

void LuaWatchWindow::updateWatchData(LuaDebugData* debugData)
{
    if (debugData && debugData->items.size() > 0)
    {
        wxTreeItemIdValue cookie;
        wxTreeItemId child = treeList->GetFirstChild(root, cookie);
        while (child.IsOk())
        {
            wxString text = treeList->GetItemText(child, 0);

            if (text == wxString::FromUTF8(debugData->items[0]->itemKey.c_str()))
            {
                treeList->SetItemText(child, 1, wxString::FromUTF8(debugData->items[0]->itemValue.c_str()));

                if (debugData->items[0]->itemValueType >= 0 &&
                    debugData->items[0]->itemValueType <= 8)
                {
                    treeList->SetItemText(child, 2, LUA_TYPE_NAME[debugData->items[0]->itemValueType]);
                }
                else
                    treeList->SetItemText(child, 2, wxEmptyString);
            }

            child = treeList->GetNextChild(root, cookie);
        }
    }
}

void LuaWatchWindow::redraw()
{
    wxTreeItemIdValue cookie;
    wxTreeItemId child = treeList->GetFirstChild(root, cookie);
    while (child.IsOk())
    {
        wxString text = treeList->GetItemText(child, 0);

        if (!text.IsEmpty())
            debugger->evaluateExpr(debugger->getCurrStackLevel(), text.ToUTF8().data());

        child = treeList->GetNextChild(root, cookie);
    }
}

bool LuaWatchWindow::isReallyShown()
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
