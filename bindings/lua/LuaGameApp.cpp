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

LuaGameApp::LuaGameApp(int argc, char* argv[]) : cmdLine(argc, argv), argv0(argv[0])
{
}

LuaGameApp::LuaGameApp(char* cmd) : cmdLine(cmd), argv0(NULL)
{
}

LuaGameApp::~LuaGameApp()
{
}

bool LuaGameApp::loadPlugins()
{
    ConfigFile plugin;
    plugin.load("plugin.cfg");

    String pluginDir = plugin.getSetting("PluginDir");
    
    StringVector plugins = plugin.getMultiSetting("Plugin");
    for (size_t i = 0; i < plugins.size(); ++i)
    {
        String fileName = pluginDir + plugins[i];
        
        if (getHareApp()->loadPlugin(fileName))
        {
            Log::getSingleton().logInfo("Load plugin : '%s'", fileName.c_str());
        }
        else
        {
            Log::getSingleton().logError("HareApp::loadPlugin failed to load '%s'", fileName.c_str());
        }
    }

    return true;
}

bool LuaGameApp::loadResources()
{
    ConfigFile resource;
    resource.load("resource.cfg");

    String writeDir = resource.getSetting("WriteDir");
    String scriptDir = resource.getSetting("ScriptDir");
    StringVector searchPaths = resource.getMultiSetting("SearchPath");

    FileSystem* fs = FileSystem::getSingletonPtr();

    fs->reset(argv0);

    fs->setWriteDir(writeDir);
    Log::getSingleton().logInfo("Filesystem write dir : '%s'", writeDir.c_str());

    fs->addSearchPath(scriptDir);
    Log::getSingleton().logInfo("Filesystem add search path : '%s'", scriptDir.c_str());

    for (size_t i = 0; i < searchPaths.size(); ++i)
    {
        fs->addSearchPath(searchPaths[i]);
        Log::getSingleton().logInfo("Filesystem add search path : '%s'", searchPaths[i].c_str());
    }

    return true;
}

bool LuaGameApp::go()
{
    core_init(argv0);

    lua_State *L = lua_open();  /* create state */
    if (L == NULL)
        return false;

    luaL_openlibs(L);

    luaopen_hare(L);

    if (!loadPlugins() || !loadResources())
    {
        lua_close(L);
        return false;
    }

    graphics_init();

    LuaDebuggee* debuggee = NULL;

    String debug = cmdLine.getOptionValue("debug");
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

    String game = cmdLine.getOptionValue("game");

    mainScript = new LuaScriptRunner();

    if (!mainScript->loadScript(game + "/script.lua"))
    {
        LuaScriptRunner::setState(NULL);
        lua_close(L);
        return false;
    }

    mainScript->callFunction("game_init");
    
    HareApp::getSingletonPtr()->hareRun();

    mainScript->callFunction("game_quit");

    if (debuggee)
    {
        debuggee->stop();
        delete debuggee;
        debuggee = 0;
    }

    LuaScriptRunner::setState(NULL);
    mainScript = 0;

    lua_close(L);

    graphics_quit();

    HareApp::getSingletonPtr()->freeAllPlugins();

    core_quit();

    return true;
}