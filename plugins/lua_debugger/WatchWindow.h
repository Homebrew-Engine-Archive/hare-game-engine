//***************************************************************
//  File:    WatchWindow.h
//  Data:    02/24/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef WATCHWINDOW_H
#define WATCHWINDOW_H

#include <wx/xrc/xmlres.h>

class LuaDebuggerPlugin;
class wxTreeListCtrl;

class LuaWatchWindow : public wxPanel
{
public:
    LuaWatchWindow(wxWindow* parent, LuaDebuggerPlugin* dbg);
    virtual ~LuaWatchWindow();

    void addEmptyRow();
    void updateWatchData(LuaDebugData* debugData);
    void updateTableData(const String& tableName, LuaDebugData* debugData);
    void redraw();

    void onDropText(const wxString& text);

protected:
    LuaDebuggerPlugin* debugger;
    wxTreeListCtrl* treeList;
    wxTreeItemId root;

private:
    void onTreeKeyDown(wxTreeEvent& event);
    void onTreeEndLabelEdit(wxTreeEvent& event);
    void onTreeBeginLabelEdit(wxTreeEvent& event);
    void onTreeItemExpanded(wxTreeEvent& event);

    DECLARE_EVENT_TABLE();
};

#endif