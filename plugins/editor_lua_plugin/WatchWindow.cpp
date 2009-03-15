#include "PCH.h"
#include "WatchWindow.h"
#include <wx/treelistctrl.h>

BEGIN_EVENT_TABLE(LuaWatchWindow, wxPanel)
END_EVENT_TABLE()

LuaWatchWindow::LuaWatchWindow(wxWindow* parent, LuaDebugger* dbg)
: wxPanel(parent), debugger(dbg)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    treeList = new wxTreeListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
        wxTR_HIDE_ROOT | wxTR_EDIT_LABELS | wxTR_FULL_ROW_HIGHLIGHT);
    sizer->Add(treeList, 1, wxALIGN_LEFT | wxALIGN_TOP | wxEXPAND, 0);
    SetSizer(sizer);
    Layout();
    sizer->Fit(this);

    treeList->AddColumn(_("Name"));
    treeList->AddColumn(_("Value"));

    root = treeList->AddRoot(wxEmptyString);

    treeList->AppendItem(root, wxEmptyString);
}

LuaWatchWindow::~LuaWatchWindow()
{
}
