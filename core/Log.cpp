//***************************************************************
//  File:    Log.cpp
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
#include "PCH.h"
#include "Log.h"
#include "Timer.h"
#include "StringUtil.h"
#include "Object.h"

namespace hare
{
    Log::Log()
    {
        firstTick = getTime();
        maxNumMessages = 1024;
        maxLogLevel = 4;
        logFileName = "EngineLog.html";
    }

    Log::~Log()
    {
        if (file.is_open())
        {
            file << "</table>\n";
            file << "</div>\n";
            file << "</body>\n";
            file << "</html>";
            file.close();
        }
    }

    void Log::pushMessage(int level, const char *msg, va_list args)
    {
        float time = getTime() - firstTick;

        if (messages.size() < maxNumMessages - 1)
        {
            messages.push(LogMessage(StringUtil::formatV(msg, args), level, time));
        }
        else if(messages.size() == maxNumMessages - 1)
        {
            messages.push(LogMessage(String("Message queue is full"), 1, time));
        }
    }

    void Log::logError(const char *msg, ...)
    {
        if (maxLogLevel < 1) return;

        va_list args;
        va_start(args, msg);
        pushMessage(1, msg, args);
        va_end(args);
    }

    void Log::logWarning(const char *msg, ...)
    {
        if (maxLogLevel < 2) return;

        va_list args;
        va_start(args, msg);
        pushMessage(2, msg, args);
        va_end(args);
    }

    void Log::logInfo(const char *msg, ...)
    {
        if (maxLogLevel < 3) return;

        va_list args;
        va_start(args, msg);
        pushMessage(3, msg, args);
        va_end(args);
    }

    void Log::logDebug(const char *msg, ...)
    {
        if (maxLogLevel < 4) return;

        va_list args;
        va_start(args, msg);
        pushMessage(4, msg, args);
        va_end(args);
    }

    bool Log::getMessage(LogMessage &msg)
    {
        if (!messages.empty())
        {
            msg = messages.front();
            messages.pop();
            return true;
        }
        else
            return false;
    }

    const char *Log::getMessage(int *level, float *time)
    {
        static String txt;
        static LogMessage msg;

        if (getMessage(msg))
        {
            if (level) *level = msg.level;
            if (time) *time = msg.time;
            return msg.text.c_str();
        }
        else
            return "";
    }

    void Log::changeFileName(const String& fileName)
    {
        if (file.is_open())
        {
            file << "</table>\n";
            file << "</div>\n";
            file << "</body>\n";
            file << "</html>";
            file.close();
        }

        logFileName = fileName;
    }

    bool Log::dumpMessages()
    {
        if (!file.is_open())
        {
            file.setf(std::ios::fixed);
            file.precision(3);
            file.open(logFileName.c_str(), std::ios::out);
            if (!file) return false;

            file << "<html>\n";
            file << "<head>\n";
            file << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
            file << "<title>Hare Log</title>\n";
            file << "<style type=\"text/css\">\n";

            file << "body, html {\n";
            file << "background: #000000;\n";
            file << "width: 1000px;\n";
            file << "font-family: Arial;\n";
            file << "font-size: 16px;\n";
            file << "color: #C0C0C0;\n";
            file << "}\n";

            file << "h1 {\n";
            file << "color : #FFFFFF;\n";
            file << "border-bottom : 1px dotted #888888;\n";
            file << "}\n";

            file << ".box {\n";
            file << "border : 1px dotted #818286;\n";
            file << "padding : 5px;\n";
            file << "margin: 5px;\n";
            file << "width: 950px;\n";
            file << "background-color : #292929;\n";
            file << "}\n";

            file << ".err {\n";
            file << "color: #EE1100;\n";
            file << "font-weight: bold\n";
            file << "}\n";

            file << ".warn {\n";
            file << "color: #FFCC00;\n";
            file << "font-weight: bold\n";
            file << "}\n";

            file << ".info {\n";
            file << "color: #C0C0C0;\n";
            file << "}\n";

            file << ".debug {\n";
            file << "color: #CCA0A0;\n";
            file << "}\n";

            file << "</style>\n";
            file << "</head>\n\n";

            file << "<body>\n";
            file << "<h1>Hare2D Game Engine Log</h1>\n";
            file << "<h3>" << getVersionString() << "</h3>\n";
            file << "<div class=\"box\">\n";
            file << "<table>\n";

            file.flush();
        }

        int level;
        float time;
        String text = getMessage(&level, &time);

        while (!text.empty())
        {
            file << "<tr>\n";
            file << "<td width=\"100\">";
            file << time;
            file << "</td>\n";
            file << "<td class=\"";

            switch(level)
            {
            case 1:
                file << "err";
                break;
            case 2:
                file << "warn";
                break;
            case 3:
                file << "info";
                break;
            default:
                file << "debug";
            }

            file << "\">";
            file << text.c_str();
            file << "</td>\n";
            file << "</tr>\n";

            file.flush();

            text = getMessage(&level, &time);
        }

        return true;
    }

}