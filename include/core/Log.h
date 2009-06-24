//***************************************************************
//  File:    Log.h
//  Data:    05/02/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _LOG_H_
#define _LOG_H_

#include "CorePrerequisites.h"
#include "Singleton.h"


namespace hare
{
    struct CORE_API LogMessage
    {
        String  text;
        int     level;
        float   time;

        LogMessage() {}

        LogMessage(const String &text, int level, float time)
            : text(text), level(level), time(time) {}
    };

    /** Class for logging messages
    */
    class CORE_API Log : public Singleton<Log>
    {
    public:
        Log();
       ~Log();

       /// Log some errors
        void logError(const char *msg, ...);
        /// Log some warnings
        void logWarning(const char *msg, ...);
        /// Log some info
        void logInfo(const char *msg, ...);
        /// Log debug messages
        void logDebug(const char *msg, ...);

        /// Get a message for message queue, return false if queue is empty.
        bool getMessage(LogMessage& msg);
        /// Get a message for message queue
        const char *getMessage(int *level, float *time);

        /// Get max messages count
        uint32 getMaxNumMessages() const
        {
            return maxNumMessages;
        }

        /// Set max messages count
        void setMaxNumMessages(uint32 maxNum)
        {
            maxNumMessages = maxNum;
        }

        /// Get max log level
        uint32 getMaxLogLevel() const
        {
            return maxLogLevel;
        }

        /// Set max log level
        void setMaxLogLevel(uint32 maxLevel)
        {
            maxLogLevel = maxLevel;
        }

        /// Change log file's name, old file will be flushed if opened.
        void changeFileName(const String& fileName);
        /// Dump messages to file in html format.
        bool dumpMessages();

    protected:
        float firstTick;
        char textBuf[2048];
        uint32 maxNumMessages;
        uint32 maxLogLevel;
        std::queue<LogMessage> messages;
        std::ofstream file;
        String logFileName;

        void pushMessage(const String &text, uint32 level);
        void pushMessage(int level, const char *msg, va_list ap);
    };
}

#endif
