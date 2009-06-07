//***************************************************************
//  File:    CallStackWindow.h
//  Data:    02/23/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef CALLSTACKWINDOW_H
#define CALLSTACKWINDOW_H

#include <wx/xrc/xmlres.h>

class LuaDebuggerPlugin;

class LuaCallStackWindow : public wxPanel
{
public:
    LuaCallStackWindow(wxWindow* parent, LuaDebuggerPlugin* dbg);
   ~LuaCallStackWindow();

    void clear();
    void setCallStackData(LuaDebugData* data);

protected:
    void onDbClick(wxListEvent& event);

    LuaDebuggerPlugin* debugger;
    LuaDebugData::Ptr callStackData;
private:
    DECLARE_EVENT_TABLE();
};

#endif