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

    // float -> String
    template <>
    inline String StringConverter::toString(const float& val)
    {
        std::ostringstream stream;
        stream.setf(std::ios::fixed, std::ios::floatfield);
        stream.precision(6);
        stream.width(0);
        stream.fill(' ');
        stream << val;
        return stream.str();
    }

    // double -> String
    template <>
    inline String StringConverter::toString(const double& val)
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

    // uint8 -> String
    template <>
    inline String StringConverter::toString(const uint8& val)
    {
        return toString(uint32(val));
    }

    // String -> uint8
    template <>
    inline uint8& StringConverter::parse(const String& str, uint8& val)
    {
        uint32 val_u32 = 0;
        parse(str, val_u32);
        val = (uint8)val_u32;
        return val;
    }

    // int8 -> String
    template <>
    inline String StringConverter::toString(const int8& val)
    {
        return toString(int32(val));
    }

    // String -> int8
    template <>
    inline int8& StringConverter::parse(const String& str, int8& val)
    {
        int32 val_int32 = 0;
        parse(str, val_int32);
        val = (int8)val_int32;
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
