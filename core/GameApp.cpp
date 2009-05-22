//***************************************************************
//  File:    GameApp.cpp
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
#include "GameApp.h"
#include "XmlVisitor.h"
#include "BinVisitor.h"
#include "AttVisitor.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(GameApp, Object, 0)
    {
    }

    GameApp::GameApp()
    {
    }

    GameApp::~GameApp()
    {
    }
}