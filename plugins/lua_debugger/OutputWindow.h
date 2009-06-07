//***************************************************************
//  File:    outputWindow.h
//  Data:    03/18/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef outputWindow_H
#define outputWindow_H

#include <wx/xrc/xmlres.h>

class LuaDebuggerPlugin;

enum LuaLoggerType
{
    Log_Print,
    Log_Info,
    Log_Error,

    Log_MaxType
};

class LuaOutputWindow : public wxPanel
{
public:
    LuaOutputWindow(wxWindow* parent, LuaDebuggerPlugin* dbg);
   ~LuaOutputWindow();

public:
    void append(const wxString& msg, LuaLoggerType tp);
    void clear();

protected:
    LuaDebuggerPlugin* debugger;
    wxTextAttr type[Log_MaxType];

private:
    DECLARE_EVENT_TABLE();
};

#endif