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
    /** Utility class for manipulating command-line. 
    */
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
        /** Parse command-line using argc and argv passed by app's main entry. 
        */
        void parseCmdLine(int argc, char* argv[]);

        /** Get parsed command-line. 
        */
        String getParsedCmdLine();

        /** Test if command-line has the option. 
        */
        bool hasOption(const String &optName);

        /** Get command-line option's value. 
        */
        String getOptionValue(const String &optName);

        /** Set command-line option's value, option will be added if not exists.
        */
        void setOptionValue(const String &optName, const String &optValue);

        /** Remove some option form command-line. 
        @return Return true if removed, false if failed or the option does not exist.
        */
        bool removeOption(const String &optName);

    private:
        typedef std::map<String, String> OptionMap;
        OptionMap cmdLineOptions;
        String cmdLine;
    };
}

#endif
