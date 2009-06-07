#include "PCH.h"
#include "LocalWindow.h"
#include "LuaDebugger.h"
#include <wx/dnd.h>
#include <wx/treelistctrl.h>

static const wxChar* LUA_TYPE_NAME[] = {
    wxT("nil"), wxT("boolean"), wxT("lightuserdata"), wxT("number"), wxT("string"),
        wxT("table"), wxT("function"), wxT("userdata"), wxT("thread")
};

BEGIN_EVENT_TABLE(LuaLocalWindow, wxPanel)
    EVT_TREE_ITEM_EXPANDED(wxID_ANY, LuaLocalWindow::onTreeItemExpanded)
END_EVENT_TABLE()

LuaLocalWindow::LuaLocalWindow(wxWindow* parent, LuaDebuggerPlugin* dbg)
: wxPanel(parent), debugger(dbg)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    treeList = new wxTreeListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTR_HIDE_ROOT | wxTR_EDIT_LABELS | wxTR_FULL_ROW_HIGHLIGHT |
        wxTR_ROW_LINES | wxTR_COLUMN_LINES | wxTR_HAS_BUTTONS);
    sizer->Add(treeList, 1, wxALIGN_LEFT | wxALIGN_TOP | wxEXPAND, 0);
    SetSizer(sizer);
    Layout();
    sizer->Fit(this);

    treeList->AddColumn(_("Name"));
    treeList->AddColumn(_("Value"), 200);
    treeList->AddColumn(_("Type"));

    root = treeList->AddRoot(wxEmptyString);
}

LuaLocalWindow::~LuaLocalWindow()
{
}

void LuaLocalWindow::onTreeItemExpanded(wxTreeEvent& event)
{
    wxTreeItemId id = event.GetItem();

    if (id.IsOk())
    {
        wxString tableName = treeList->GetItemText(id, 0);

        debugger->enumerateTable(debugger->getCurrStackLevel(), tableName.ToUTF8().data());
    }
    else
        event.Veto();
}

void LuaLocalWindow::updateTableData(const String& tableName, LuaDebugData* debugData)
{
    if (debugData && debugData->items.size() > 0)
    {
        wxTreeItemIdValue cookie;
        wxTreeItemId child = treeList->GetFirstChild(root, cookie);
        while (child.IsOk())
        {
            wxString text = treeList->GetItemText(child, 0);

            if (text == wxString::FromUTF8(tableName.c_str()))
            {
                treeList->DeleteChildren(child);

                for (size_t i = 0; i < debugData->items.size(); ++i)
                {
                    wxString key = wxString::FromUTF8(debugData->items[i]->itemKey.c_str());
                    wxString value = wxString::FromUTF8(debugData->items[i]->itemValue.c_str());
                    wxString type = LUA_TYPE_NAME[debugData->items[i]->itemValueType];

                    wxTreeItemId itemId = treeList->AppendItem(child, key);
                    treeList->SetItemText(itemId, 1, value);
                    treeList->SetItemText(itemId, 2, type);
                }
            }

            child = treeList->GetNextChild(root, cookie);
        }
    }
}

void LuaLocalWindow::updateLocalData(LuaDebugData* debugData)
{
    wxString key;
    wxString value;

    treeList->DeleteChildren(root);

    if (debugData && debugData->items.size() > 0)
    {
        for (size_t i = 0; i < debugData->items.size(); ++i)
        {
            key = wxString::FromUTF8(debugData->items[i]->itemKey.c_str());
            value = wxString::FromUTF8(debugData->items[i]->itemValue.c_str());
            key = wxString::FromUTF8(debugData->items[i]->itemKey.c_str());
            wxTreeItemId child = treeList->AppendItem(root, key);
            treeList->SetItemText(child, 1, value);
            if (debugData->items[i]->itemValueType >= 0 &&
                debugData->items[i]->itemValueType <= 8)
            {
                treeList->SetItemText(child, 2, LUA_TYPE_NAME[debugData->items[i]->itemValueType]);
            }

            if (debugData->items[i]->itemValueType == LUA_TTABLE)
            {
                treeList->AppendItem(child, wxEmptyString);
            }
        }
    }
}
