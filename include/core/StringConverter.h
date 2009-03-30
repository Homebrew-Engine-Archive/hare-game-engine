//***************************************************************
//  File:    StringConverter.h
//  Date:    10/03/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _STRINGCONVERTER_H_
#define _STRINGCONVERTER_H_

#include "CorePrerequisites.h"
#include "StringUtil.h"

namespace hare_core
{
    class CORE_API StringConverter
    {
    public:
        template <typename T>
        static String toString(const T &val)
        {
            std::ostringstream stream;
            stream << val;
            return stream.str();
        }

        template <typename T>
        static T& parse(const String& str, T& val)
        {
            std::istringstream stream(str);
            stream >> val;
            return val;
        }
    };

    // String -> String
    template <>
    inline String StringConverter::toString(const String& val)
    {
        return val;
    }

    // f32 -> String
    template <>
    inline String StringConverter::toString(const f32& val)
    {
        std::ostringstream stream;
        stream.setf(std::ios::fixed, std::ios::floatfield);
        stream.precision(6);
        stream.width(0);
        stream.fill(' ');
        stream << val;
        return stream.str();
    }

    // f64 -> String
    template <>
    inline String StringConverter::toString(const f64& val)
    {
        std::ostringstream stream;
        stream.setf(std::ios::fixed, std::ios::floatfield);
        stream.precision(15);
        stream.width(0);
        stream.fill(' ');
        stream << val;
        return stream.str();
    }

    // String -> String
    template <>
    inline String& StringConverter::parse(const String& str, String& val)
    {
        val = str;
        return val;
    }

    // u8 -> String
    template <>
    inline String StringConverter::toString(const u8& val)
    {
        return toString(u32(val));
    }

    // String -> u8
    template <>
    inline u8& StringConverter::parse(const String& str, u8& val)
    {
        u32 val_u32 = 0;
        parse(str, val_u32);
        val = (u8)val_u32;
        return val;
    }

    // s8 -> String
    template <>
    inline String StringConverter::toString(const s8& val)
    {
        return toString(s32(val));
    }

    // String -> s8
    template <>
    inline s8& StringConverter::parse(const String& str, s8& val)
    {
        s32 val_s32 = 0;
        parse(str, val_s32);
        val = (s8)val_s32;
        return val;
    }

    // SizeF -> String
    template <>
    inline String StringConverter::toString(const SizeF& val)
    {
        return toString(val.cx) + " " + toString(val.cy);
    }

    // PointF -> String
    template <>
    inline String StringConverter::toString(const PointF& val)
    {
        return toString(val.x) + " " + toString(val.y);
    }

    // String -> SizeF 
    template <>
    inline SizeF& StringConverter::parse(const String& str, SizeF& val)
    {
        StringVector strs = StringUtil::split(str, " ");
        assert(strs.size() == 2);
        parse(strs[0], val.cx);
        parse(strs[1], val.cy);
        return val;
    }

    // String -> PointF 
    template <>
    inline PointF& StringConverter::parse(const String& str, PointF& val)
    {
        StringVector strs = StringUtil::split(str, " ");
        assert(strs.size() == 2);
        parse(strs[0], val.x);
        parse(strs[1], val.y);
        return val;
    }

}

#endif
