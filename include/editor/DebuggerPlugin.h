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

namespace hare
{
    class EDITOR_API DebuggerPlugin : public EditorPlugin
    {
    public:
        DebuggerPlugin();
        virtual ~DebuggerPlugin();
    public:
        virtual bool start() = 0;
        virtual bool addBreakPoint(const String &fileName, int lineNumber) = 0;
        virtual bool removeBreakPoint(const String &fileName, int lineNumber) = 0;
    };

}

#endif
