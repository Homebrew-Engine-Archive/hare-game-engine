//***************************************************************
//  File:    LuaScriptRunner.h
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
#ifndef LUASCRIPTRUNNER_H
#define LUASCRIPTRUNNER_H

class LuaScriptRunner : public ScriptRunner
{
    HARE_DECLARE_DYNAMIC_CLASS(LuaScriptRunner)
public:
    LuaScriptRunner();

    static void setState(lua_State *L)
    {
        luaState = L;
    }

    static lua_State* getState()
    {
        return luaState;
    }

    String getLastError(bool clear = true)
    {
        String err = error;
        if (clear) error.clear();
        return err;
    }

    bool loadScript(const String& fileName = StringUtil::EMPTY);

    bool callFunction(const String& name);

    virtual bool notifyOwnerCreated();
    virtual bool notifyOwnerDestroyed();

    virtual void postLoaded();

protected:

    // make the 'global variables' stored in a new envrionment table instead of _G
    void scopeScript();

private:

    static lua_State* luaState;

    int envRef;

    String error;
};

#endif