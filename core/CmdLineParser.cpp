//***************************************************************
//  File:    CmdLineParser.cpp
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
#include "PCH.h"
#include "CmdLineParser.h"
#include "StringUtil.h"

namespace hare_core
{
    CmdLineParser::CmdLineParser()
    {
    }

    CmdLineParser::CmdLineParser(int argc, char* argv[])
    {
        parseCmdLine(argc, argv);
    }

    void CmdLineParser::parseCmdLine(int argc, char* argv[])
    {
        cmdLineOptions.clear();
        cmdLine = StringUtil::EMPTY;

        String optName, optValue;

        for (int i = 0; i < argc; ++i)
        {
            char* arg = argv[i];

            if (arg[0] == '-')
                optName = arg + 1;
            else
                continue;

            if (i + 1 < argc)
            {
                if (argv[i + 1][0] != '-')
                {
                    optValue = argv[i + 1];
                    ++i;
                }
                else
                    optValue = StringUtil::EMPTY;
            }
            else
                optValue = StringUtil::EMPTY;

            cmdLineOptions[optName] = optValue;
            cmdLine += "-" + optName + " " + optValue + " ";
        }
    }

    String CmdLineParser::getParsedCmdLine()
    {
        return cmdLine;
    }

    bool CmdLineParser::hasOption(const String &optName)
    {
        return cmdLineOptions.find(optName) != cmdLineOptions.end();
    }

    String CmdLineParser::getOptionValue(const String &optName)
    {
        OptionMap::iterator it = cmdLineOptions.find(optName);
        if (it != cmdLineOptions.end())
            return it->second;
        else
            return StringUtil::EMPTY;
    }

    void CmdLineParser::setOptionValue(const String &optName, const String &optValue)
    {
        cmdLineOptions[optName] = optValue;
    }

    bool CmdLineParser::removeOption(const String &optName)
    {
        OptionMap::iterator it = cmdLineOptions.find(optName);
        if (it != cmdLineOptions.end())
        {
            cmdLineOptions.erase(it);
            return true;
        }
        else
            return false;
    }
}
