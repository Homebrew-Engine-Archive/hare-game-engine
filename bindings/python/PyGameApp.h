//***************************************************************
//  File:    PyGameApp.h
//  Data:    05/29/2009
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

class PyGameApp : public GameApp
{
    HARE_DECLARE_DYNAMIC_CLASS(PyGameApp)

public:
    virtual bool go();

protected:
};

#endif