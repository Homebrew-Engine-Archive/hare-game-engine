//***************************************************************
//  File:    Types.h
//  Date:    10/12/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _TYPES_H__
#define _TYPES_H__

#include "MathCommon.h"

namespace hare_core
{
    typedef unsigned char	uint8;
    typedef signed char		int8;
    typedef unsigned short	uint16;
    typedef signed short	int16;
    typedef unsigned int	uint32;
    typedef signed int		int32;

    typedef std::string     String;
    typedef std::basic_string<wchar_t> WString;
    typedef std::vector<String> StringVector;

    typedef TPoint<float>     PointF;
    typedef TPoint<int32>     PointN;

    typedef TSize<float>      SizeF;
    typedef TSize<int32>      SizeN;

    typedef TRect<float>      RectF;
    typedef TRect<int32>      RectN;
}

#if HARE_COMPILER == HARE_COMPILER_GNUC && HARE_COMP_VER >= 310 && !defined(STLPORT)

namespace __gnu_cxx
{
    template <> struct hash<hare_core::String>
    {
        size_t operator()(const hare_core::String &str) const
        {
            register size_t ret = 0;
            for (hare_core::String::const_iterator it = str.begin(); it != str.end(); ++it)
                ret = 5 * ret + *it;

            return ret;
        }
    };
}

#endif

#endif
