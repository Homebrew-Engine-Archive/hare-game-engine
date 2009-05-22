//***************************************************************
//  File:    StringUtil.h
//  Date:    09/30/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

#include "CorePrerequisites.h"

namespace hare
{
    class CORE_API StringUtil
    {
    public:
        static void toLowerCase(String &str);
        static void toUpperCase(String &str);

        static String replace(const String &str, const String& olds, const String& news);
        static StringVector split(const String &str, const String& delims = "\t\n\r ");

        static String format(const char* formats, ...);
        static String formatV(const char* formats, va_list ap);

        static void trim(String& str, bool left = true, bool right = true);
		static void splitFilename(const String& fullName, String& outBasename, String& outExtention);

        static String toUTF8(const WString& str);
        static WString fromUTF8(const String& str);

        static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);
        static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

        static const String EMPTY;
    };
}

#endif
