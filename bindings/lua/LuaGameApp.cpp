//***************************************************************
//  File:    LuaGameApp.cpp
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
#include "PCH.h"
#include "LuaGameApp.h"
#include "LuaDebuggee.h"

extern "C"
{
    int luaopen_hare(lua_State *L);
}

bool notify_error(lua_State *L);

HARE_IMPLEMENT_DYNAMIC_CLASS(LuaGameApp, Object, 0)
{
}

bool LuaGameApp::go()
{
    lua_State *L = lua_open();  /* create state */
    if (L == NULL)
        return false;

    luaL_openlibs(L);

    luaopen_hare(L);

    LuaDebuggee* debuggee = NULL;

    String debug = CmdLineParser::getSingletonPtr()->getOptionValue("debug");
    if (!debug.empty())
    {
        StringVector cmds = StringUtil::split(debug, ":");
        if (cmds.size() == 2)
        {
            String addr = cmds[0];
            int port = -1;
            StringConverter::parse(cmds[1], port);
            debuggee = new LuaDebuggee(L, addr, port);
            debuggee->start();
        }
    }

    LuaScriptRunner::setState(L);

    String game = CmdLineParser::getSingletonPtr()->getOptionValue("game");

    mainScript = new LuaScriptRunner();

    String gameScript;

    // Script url MUST start with '/' for debugging.
    if (game.empty()) 
        gameScript = "/script.lua";
    else
        gameScript = "/" + game + "/script.lua";

    if (!mainScript->loadScript(gameScript))
    {
        notify_error(L);
        LuaScriptRunner::setState(NULL);
        lua_close(L);
        return false;
    }

    if (!mainScript->notifyOwnerCreated())
        notify_error(L);
    
    HareApp::getSingletonPtr()->hareRun();

    if (!mainScript->notifyOwnerDestroyed())
        notify_error(L);

    // Destroy all lua objects first.
    lua_close(L);

    if (debuggee)
    {
        debuggee->stop();
        delete debuggee;
        debuggee = 0;
    }

    mainScript = 0;
    LuaScriptRunner::setState(NULL);

    return true;
}