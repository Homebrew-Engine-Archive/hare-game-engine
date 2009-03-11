//***************************************************************
//  File:    DebuggerPlugin.h
//  Data:    11/03/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef _DEBUGGERPLUGIN_H_
#define _DEBUGGERPLUGIN_H_

#include "EditorPrerequisites.h"
#include "EditorPlugin.h"

namespace hare_editor
{
    class EDITOR_API DebuggerPlugin : public EditorPlugin
    {
    public:
        DebuggerPlugin();
        virtual ~DebuggerPlugin();
    };

}

#endif
