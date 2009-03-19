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

class LuaDebugger;

class LuaCallStackWindow : public wxPanel
{
public:
    LuaCallStackWindow(wxWindow* parent, LuaDebugger* dbg);
   ~LuaCallStackWindow();

    void clear();
    void setCallStackData(LuaDebugData* data);
    bool isReallyShown();

protected:
    void onDbClick(wxListEvent& event);

    LuaDebugger* debugger;
    LuaDebugData::Ptr callStackData;
private:
    DECLARE_EVENT_TABLE();
};

#endif