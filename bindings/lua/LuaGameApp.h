//***************************************************************
//  File:    LuaGameApp.h
//  Data:    05/21/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef LUA_GAME_APP_H
#define LUA_GAME_APP_H

#include "LuaScriptRunner.h"

class LuaGameApp
{
public:
    LuaGameApp(int argc, char* argv[]);
    LuaGameApp(char* cmd);

    bool go();

protected:
    bool loadPlugins();
    bool loadResources();

    LuaScriptRunner::Ptr mainScript;
    CmdLineParser cmdLine;
    const char* argv0;
};

#endif