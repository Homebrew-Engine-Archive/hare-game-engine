//***************************************************************
//  File:    LuaScriptRunner.cpp
//  Data:    05/20/2009
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
#include "LuaScriptRunner.h"

void dynamicCastObject(lua_State* L, Object* object, int owner);

lua_State* LuaScriptRunner::luaState = 0;

HARE_IMPLEMENT_DYNAMIC_CLASS(LuaScriptRunner, ScriptRunner, 0)
{
}

LuaScriptRunner::LuaScriptRunner() : envRef(-1)
{

}

bool LuaScriptRunner::loadScript(const String& fileName)
{
    if (!fileName.empty())
    {
        scriptFile = fileName;
    }

    if (!luaState)
    {
        error = "LuaScriptRunner::loadScript() luaState is empty.";
        return false;
    }

    FileSystem* fs = FileSystem::getSingletonPtr();

    FileHandle fh = fs->openFile(scriptFile, FM_Read);
    if (!fh)
    {
        error = "LuaScriptRunner::loadScript() can not open file : " + scriptFile;
        return false;
    }

    int size = fs->size(fh);
    if (size <= 0)
    {
        fs->closeFile(fh);
        error = "LuaScriptRunner::loadScript() empty file : " + scriptFile;
        return false;
    }

    char* buffer = new char[size];
    fs->readFile(fh, buffer, size, 1);

    int status = luaL_loadbuffer(luaState, buffer, size, scriptFile.c_str());
    if (status)
    {
        error = StringUtil::format("LuaScriptRunner::loadScript() luaL_loadbuffer failed (%d), err : %s",
            status, luaL_checkstring(luaState, -1));
    }
    else
    {
        scopeScript();
        status = lua_pcall(luaState, 0, LUA_MULTRET, 0);
        if (status)
        {
            error = StringUtil::format("LuaScriptRunner::loadScript() lua_pcall failed (%d), err : %s",
                status, luaL_checkstring(luaState, -1));
        }
    }

    delete [] buffer;

    return status == 0;
}

void LuaScriptRunner::scopeScript()
{
    /* local env = {}
       setmetatable(env, {__index = _G})
       setfenv(1, env)  
    */
    lua_newtable(luaState);                     // env

    lua_pushvalue(luaState, -1);
    envRef = luaL_ref(luaState, LUA_REGISTRYINDEX);

    lua_newtable(luaState);                     // metatable
    lua_pushstring(luaState, "__index");        // key
    lua_pushvalue(luaState, LUA_GLOBALSINDEX);  // value
    lua_rawset(luaState, -3);                   // metatable[key] = value, pops key and value, 
                                                // so metatable is on stack top
    lua_setmetatable(luaState, -2);             // set env's metatable, pops metatable
    lua_setfenv(luaState, -2);                  // setfenv for the function on top pushed by luaL_loadbuffer
}

bool LuaScriptRunner::callFunction(const String& name)
{
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, envRef);
    lua_getfield(luaState, -1, name.c_str());

    dynamicCastObject(luaState, owner, 0);

    int status = lua_pcall(luaState, 1, LUA_MULTRET, 0);

    if (status)
    {
        error = StringUtil::format("LuaScriptRunner::callFunction() lua_pcall failed (%d), err : %s",
            status, luaL_checkstring(luaState, -1));
    }

    return status == 0;
}

void LuaScriptRunner::postLoaded()
{
    if (startAfterLoaded && !scriptFile.empty())
    {
        loadScript();
    }
}

