//***************************************************************
//  File:    StringUtil.cpp
//  Date:    10/01/2008
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
#include "StringUtil.h"
#include "ConvertUTF.h"

namespace hare_core
{
    const String StringUtil::EMPTY;

    void StringUtil::toLowerCase(String& str)
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
            tolower);
    }

    void StringUtil::toUpperCase(String& str)
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
            toupper);
    }

    String StringUtil::replace(const String &str, const String& olds, const String& news)
    {
        assert(!olds.empty());

        if (olds == news)
            return str;

        String out = str;
        size_t pos;
        pos = str.find(olds, 0);
        while (pos != String::npos)
        {
            out.replace(pos, olds.size(), news);
            pos = out.find(olds, pos+news.size());
        }
        return out;
    }

    StringVector StringUtil::split(const String& str, const String& delims)
    {
        StringVector out;
        // Pre-allocate some space for performance
        out.reserve(10);

        size_t pos, start, end;
        start = 0;

        do
        {
            pos = str.find_first_not_of(delims, start);
            if (pos != String::npos)
            {
                end = str.find_first_of(delims, pos);
                if (end == String::npos)
                    end = str.size();
                out.push_back(str.substr(pos, end-pos));

                start = end;
            }
        }
        while (pos != String::npos);

        return out;
    }

    String StringUtil::format(const char* formats, ...)
    {
        va_list list;
        va_start(list, formats);

        //@FIXME
        char buff[4097];
        vsprintf(buff, formats, list);
        va_end(list);

        return buff;
    }

    void StringUtil::trim(String& str, bool left, bool right)
    {
        static const String delims = " \t\r";
        if (right)
            str.erase(str.find_last_not_of(delims) + 1); // trim right
        if (left)
            str.erase(0, str.find_first_not_of(delims)); // trim left
    }

	void StringUtil::splitFilename(const String& fullName,
		String& outBasename, String& outExtention)
	{
		size_t i = fullName.find_last_of(".");
		if (i == String::npos)
		{
			outExtention.clear();
			outBasename = fullName;
		}
		else
		{
			outExtention = fullName.substr(i + 1);
			outBasename = fullName.substr(0, i);
		}
	}

    template <int T> String _toUTF8(const WString& str)
    {
        assert(false);
        return "";
    }

    template <int T> WString _fromUTF8(const String& str)
    {
        assert(false);
        return L"";
    }

    template<> String _toUTF8<2>(const WString& str)
    {
        size_t size = 3 * str.length() + 1;
        String ret(size, 0);
        const UTF16* srcSta = (const UTF16*)str.c_str();
		const UTF16* srcEnd = srcSta + str.length();
        UTF8* tarSta = (UTF8*)ret.c_str();
        UTF8* tarEnd = tarSta + size;
        ConversionResult res = ConvertUTF16toUTF8(&srcSta, srcEnd, &tarSta, tarEnd, strictConversion);
        if (res != conversionOK)
        {
            assert(false);
        }
        return ret.c_str();
    }

    template<> String _toUTF8<4>(const WString& str)
    {
        size_t size = 4 * str.length() + 1;
        String ret(size, 0);
		const UTF32* srcSta = (const UTF32*)str.c_str();
		const UTF32* srcEnd = srcSta + str.length();
        UTF8* tarSta = (UTF8*)ret.c_str();
        UTF8* tarEnd = tarSta + size;
        ConversionResult res = ConvertUTF32toUTF8(&srcSta, srcEnd, &tarSta, tarEnd, strictConversion);
        if (res != conversionOK)
        {
            assert(false);
        }
        return ret.c_str();
    }

    template<> WString _fromUTF8<2>(const String& str)
    {
        size_t size = str.size() + 1;
        WString ret(size, 0);
        const UTF8* srcSta = (const UTF8*)str.c_str();
		const UTF8* srcEnd = srcSta + str.length();
        UTF16* tarSta = (UTF16*)ret.c_str();
        UTF16* tarEnd = tarSta + size;
        ConversionResult res = ConvertUTF8toUTF16(&srcSta, srcEnd, &tarSta, tarEnd, strictConversion);
        if (res != conversionOK)
        {
            assert(false);
        }
        return ret.c_str();
    }

    template<> WString _fromUTF8<4>(const String& str)
    {
        size_t size = str.size() + 1;
        WString ret(size, 0);
        const UTF8* srcSta = (const UTF8*)str.c_str();
        const UTF8* srcEnd = srcSta + str.length();
        UTF32* tarSta = (UTF32*)ret.c_str();
        UTF32* tarEnd = tarSta + size;
        ConversionResult res = ConvertUTF8toUTF32(&srcSta, srcEnd, &tarSta, tarEnd, strictConversion);
        if (res != conversionOK)
        {
            assert(false);
        }
        return ret.c_str();
    }

    String StringUtil::toUTF8(const WString& str)
    {
        enum { T = sizeof(WString::value_type) };
        return _toUTF8<T>(str);
    }

    WString StringUtil::fromUTF8(const String& str)
    {
        enum { T = sizeof(WString::value_type) };
        return _fromUTF8<T>(str);
    }
}
