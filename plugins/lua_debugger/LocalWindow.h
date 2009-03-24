//***************************************************************
//  File:    LocalWindow.h
//  Data:    03/23/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef LOCALWINDOW_H
#define LOCALWINDOW_H

#include <wx/xrc/xmlres.h>

class LuaDebugger;
class wxTreeListCtrl;

class LuaLocalWindow : public wxPanel
{
public:
    LuaLocalWindow(wxWindow* parent, LuaDebugger* dbg);
    virtual ~LuaLocalWindow();

    void updateLocalData(LuaDebugData* debugData);
    void updateTableData(const String& tableName, LuaDebugData* debugData);

protected:
    LuaDebugger* debugger;
    wxTreeListCtrl* treeList;
    wxTreeItemId root;

private:
    void onTreeItemExpanded(wxTreeEvent& event);

    DECLARE_EVENT_TABLE();
};

#endif