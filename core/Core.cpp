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
#include "Log.h"

namespace hare
{
    void CORE_API core_init(const char* argv0)
    {
        static Log theLog;
        static FileSystem theFileSystem(argv0);
		MathUtil::rand_seed(0);
        // Set the numeric locale to C to avoid ','
        setlocale(LC_NUMERIC, "C");

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
        WSADATA wsaData;
        WORD wVersionRequested = MAKEWORD(1, 1);
        WSAStartup(wVersionRequested, &wsaData);
#endif
    }

    void CORE_API core_quit()
    {
        Log::getSingleton().dumpMessages();

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
        WSACleanup();
#endif
    }
}
