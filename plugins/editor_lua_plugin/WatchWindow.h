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

class LuaDebugger;
class wxTreeListCtrl;

class LuaWatchWindow : public wxPanel
{
public:
    LuaWatchWindow(wxWindow* parent, LuaDebugger* dbg);
    virtual ~LuaWatchWindow();

protected:
    LuaDebugger* debugger;
    wxTreeListCtrl* treeList;
    wxTreeItemId root;
private:
    DECLARE_EVENT_TABLE();
};

#endif