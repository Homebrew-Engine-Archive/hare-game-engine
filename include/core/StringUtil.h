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
    /**  Utility class for manipulating Strings.
    */
    class CORE_API StringUtil
    {
    public:
        /** Lower-cases all the characters in the string.
        */
        static void toLowerCase(String &str);
        
        /** Upper-cases all the characters in the string.
        */
        static void toUpperCase(String &str);

        /** Replace all old string with new string in the string.
        @param
            olds String to be replaced by.
        @param
            news String to be replaced with.
        */
        static String replace(const String &str, const String& olds, const String& news);

        /** Returns a StringVector that contains all the substrings delimited
            by the characters in the passed <code>delims</code> argument.
        @param
            delims A list of delimiter characters to split by
        */
        static StringVector split(const String &str, const String& delims = "\t\n\r ");

        /** Similar to the standard function sprintf(). 
        */
        static String format(const char* formats, ...);
        
        /** Same as format, but takes va_list as argument.
        */
        static String formatV(const char* formats, va_list ap);

        /** Removes any whitespace characters, be it standard space or
            TABs and so on.
        @remarks
            The user may specify wether they want to trim only the
            beginning or the end of the String (the default action is
            to trim both).
        */
        static void trim(String& str, bool left = true, bool right = true);

        /** Method for splitting a fully qualified filename into the base name and extention.
        */
        static void splitFilename(const String& fullName, String& outBasename, String& outExtention);

        /** Convert from wide string to utf8 string.
        */
        static String toUTF8(const WString& str);

        /** Convert from utf8 string to wide string.
        */
        static WString fromUTF8(const String& str);

        /** Returns whether the string begins with the pattern passed in.
        @param 
            pattern The pattern to compare with.
        @param 
            lowerCase If true, the end of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

        /** Returns whether the string ends with the pattern passed in.
        @param 
            pattern The pattern to compare with.
        @param 
            lowerCase If true, the end of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

        static const String EMPTY;
    };
}

#endif
