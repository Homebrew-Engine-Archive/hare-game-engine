//***************************************************************
//  File:    PluginMain.cpp
//  Data:    01/17/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#include "PCH.h"
#include "LuaDebugger.h"

static const PluginInfo luaDebugger = {
    wxT("LuaDebugger"), 
    wxT("LuaDebugger"),
    wxT("This is a plugin for lua debugging") };

PluginRegistrant<LuaDebuggerPlugin> regLuaDebugger(luaDebugger);
