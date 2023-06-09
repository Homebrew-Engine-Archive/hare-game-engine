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

class LuaGameApp : public GameApp
{
    HARE_DECLARE_DYNAMIC_CLASS(LuaGameApp)

public:
    virtual bool go();

protected:
    LuaScriptRunner::Ptr mainScript;
};

#endif