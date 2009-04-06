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
#include "Socket.h"
#include "MathUtil.h"

namespace hare_core
{
    CORE_API void core_init(const char* argv0)
    {
        static FileSystem theFileSystem(argv0);
		MathUtil::rand_seed(0);

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
        WSADATA wsaData;
        WORD wVersionRequested = MAKEWORD(1, 1);
        WSAStartup(wVersionRequested, &wsaData);
#endif
    }

    CORE_API void core_quit()
    {
#if HARE_PLATFORM == HARE_PLATFORM_WIN32
        WSACleanup();
#endif
    }
}
