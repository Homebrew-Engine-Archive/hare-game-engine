//***************************************************************
//  File:    Core.cpp
//  Date:    01/02/2009
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
#include "FileSystem.h"
#include "StringUtil.h"
#include "CmdLineParser.h"

namespace hare_core
{
    CORE_API void core_init(const char* argv0)
    {
        static FileSystem theFileSystem(argv0);
    }

    CORE_API void core_quit()
    {
    }
}
