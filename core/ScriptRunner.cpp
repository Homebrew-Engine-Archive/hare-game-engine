//***************************************************************
//  File:    ScriptRunner.cpp
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
#include "ScriptRunner.h"
#include "ClassInfo.h"
#include "XmlVisitor.h"
#include "BinVisitor.h"
#include "AttVisitor.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(ScriptRunner, Object, 0)
    {
        HARE_META_F(scriptFile, String, propFSUrl)
        HARE_META(startAfterLoaded, bool)
    }

    ScriptRunner::ScriptRunner() : owner(0), startAfterLoaded(false)
    {
    }

    ScriptRunner::~ScriptRunner()
    {
    }
}