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

        LogMessage(String &text, int level, float time)
            : text(text), level(level), time(time) {}
    };

    class CORE_API Log : public Singleton<Log>
    {
    public:
        Log();
        ~Log();

        void logError(const char *msg, ...);
        void logWarning(const char *msg, ...);
        void logInfo(const char *msg, ...);
        void logDebug(const char *msg, ...);

        bool getMessage(LogMessage& msg);
        const char *getMessage(int *level, float *time);

        uint32 getMaxNumMessages() const
        { 
            return maxNumMessages; 
        }
        
        void setMaxNumMessages(uint32 maxNum) 
        { 
            maxNumMessages = maxNum; 
        }

        uint32 getMaxLogLevel() const
        {
            return maxLogLevel;
        }

        void setMaxLogLevel(uint32 maxLevel)
        {
            maxLogLevel = maxLevel;
        }

        bool dumpMessages();

    protected:
        float firstTick;
        char textBuf[2048];
        uint32 maxNumMessages;
        uint32 maxLogLevel;
        std::queue<LogMessage> messages;
        std::ofstream file;

        void pushMessage(const String &text, uint32 level);
        void pushMessage(int level, const char *msg, va_list ap);
    };
}

#endif