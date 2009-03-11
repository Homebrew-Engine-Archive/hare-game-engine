//***************************************************************
//  File:    CompilerPlugin.h
//  Data:    11/04/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef _COMPILERPLUGIN_H_
#define _COMPILERPLUGIN_H_

#include "EditorPrerequisites.h"
#include "EditorPlugin.h"

namespace hare_editor
{
    class EDITOR_API CompilerPlugin : public EditorPlugin
    {
    public:
        CompilerPlugin();
        ~CompilerPlugin();

    public:
        int compile();

    };
}

#endif
