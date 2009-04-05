#include "PCH.h"
#include "WatchWindow.h"
#include "LuaDebugger.h"
#include <wx/dnd.h>
#include <wx/treelistctrl.h>

static const wxChar* LUA_TYPE_NAME[] = {
    wxT("nil"), wxT("boolean"), wxT("lightuserdata"), wxT("number"), wxT("string"),
    wxT("table"), wxT("function"), wxT("userdata"), wxT("thread")
};

class TextDropTarget : public wxTextDropTarget
{
public:
    TextDropTarget(LuaWatchWindow *pOwner) { owner = pOwner; }

    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text)
    {
        owner->onDropText(text);

        return false;
    }

private:
    LuaWatchWindow *owner;
};

BEGIN_EVENT_TABLE(LuaWatchWindow, wxPanel)
    EVT_TREE_KEY_DOWN(wxID_ANY, onTreeKeyDown)
    EVT_TREE_END_LABEL_EDIT(wxID_ANY, onTreeEndLabelEdit)
    EVT_TREE_BEGIN_LABEL_EDIT(wxID_ANY, onTreeBeginLabelEdit)
    EVT_TREE_ITEM_EXPANDED(wxID_ANY, onTreeItemExpanded)
END_EVENT_TABLE()

LuaWatchWindow::LuaWatchWindow(wxWindow* parent, LuaDebugger* dbg)
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

    treeList->AddColumn(_("Name"), 150, wxALIGN_LEFT, -1, true, true);
    treeList->AddColumn(_("Value"), 150);
    treeList->AddColumn(_("Type"));

    SetDropTarget(new TextDropTarget(this));

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

void LuaWatchWindow::onDropText(const wxString& text)
{
    if (text.IsEmpty())
        return;

    treeList->InsertItem(root, 0, text);

    debugger->evaluateExpr(debugger->getCurrStackLevel(), text.ToUTF8().data());
}

void LuaWatchWindow::onTreeItemExpanded(wxTreeEvent& event)
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

void LuaWatchWindow::onTreeKeyDown(wxTreeEvent& event)
{
    if (event.GetKeyCode() == WXK_F2)
    {
        treeList->EditLabel(treeList->GetSelection(), 0);
    }
    else if (event.GetKeyCode() == WXK_DELETE)
    {
        wxTreeItemIdValue cookie;
        wxTreeItemId id = treeList->GetSelection();
        if (id && id != treeList->GetLastChild(root, cookie) &&
            treeList->GetItemParent(id) == root)
        {
            treeList->Delete(id);
            event.Veto();
            return;
        }
    }

    event.Skip();
}

void LuaWatchWindow::onTreeBeginLabelEdit(wxTreeEvent& event)
{
    if (treeList->GetItemParent(event.GetItem()) != root)
        event.Veto();
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

void LuaWatchWindow::updateTableData(const String& tableName, LuaDebugData* debugData)
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
                bool isExpnded = treeList->IsExpanded(child);
                treeList->DeleteChildren(child);
                wxString value = wxString::FromUTF8(debugData->items[0]->itemValue.c_str());
                treeList->SetItemText(child, 1, value);

                if (debugData->items[0]->itemValueType >= 0 &&
                    debugData->items[0]->itemValueType <= 8)
                {
                    treeList->SetItemText(child, 2, LUA_TYPE_NAME[debugData->items[0]->itemValueType]);

                    if (debugData->items[0]->itemValueType == LUA_TTABLE)
                    {
                        treeList->AppendItem(child, wxEmptyString);

                        if (isExpnded)
                            debugger->enumerateTable(debugger->getCurrStackLevel(), text.ToUTF8().data());
                    }
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
        treeList->DeleteChildren(child);
        treeList->SetItemText(child, 1, wxEmptyString);
        treeList->SetItemText(child, 2, wxEmptyString);

        wxString text = treeList->GetItemText(child, 0);

        if (!text.IsEmpty())
            debugger->evaluateExpr(debugger->getCurrStackLevel(), text.ToUTF8().data());

        child = treeList->GetNextChild(root, cookie);
    }
}
