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

    template <>
    inline String StringConverter::toString(const String& val)
    {
        return val;
    }

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

    template <>
    inline String StringConverter::toString(const f64& val)
    {
        std::ostringstream stream;
        stream.setf(std::ios::fixed, std::ios::floatfield);
        stream.precision(6);
        stream.width(0);
        stream.fill(' ');
        stream << val;
        return stream.str();
    }

    template <>
    inline String& StringConverter::parse(const String& str, String& val)
    {
        val = str;
        return val;
    }

    template <>
    inline String StringConverter::toString(const u8& val)
    {
        return toString(u32(val));
    }

    template <>
    inline u8& StringConverter::parse(const String& str, u8& val)
    {
        u32 val_u32 = 0;
        parse(str, val_u32);
        val = (u8)val_u32;
        return val;
    }

    template <>
    inline String StringConverter::toString(const s8& val)
    {
        return toString(s32(val));
    }

    template <>
    inline s8& StringConverter::parse(const String& str, s8& val)
    {
        s32 val_s32 = 0;
        parse(str, val_s32);
        val = (s8)val_s32;
        return val;
    }
}

#endif
