//***************************************************************
//  File:    CmdLineParser.h
//  Data:    02/14/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

#include "CorePrerequisites.h"
#include "Singleton.h"

namespace hare
{
    class CORE_API CmdLineParser : public Singleton<CmdLineParser>
    {
        HARE_DECLARE_SINGLETON(CmdLineParser)

    public:
        CmdLineParser();
        CmdLineParser(int argc, char* argv[]);

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
        CmdLineParser(char* cmd);
#endif

    public:
        void parseCmdLine(int argc, char* argv[]);

        String getParsedCmdLine();

        bool hasOption(const String &optName);

        String getOptionValue(const String &optName);

        void setOptionValue(const String &optName, const String &optValue);

        bool removeOption(const String &optName);

    private:
        typedef std::map<String, String> OptionMap;
        OptionMap cmdLineOptions;
        String cmdLine;
    };
}

#endif
