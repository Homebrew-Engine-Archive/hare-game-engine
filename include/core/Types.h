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
#ifndef _TYPES_H_
#define _TYPES_H_

#include "MathCommon.h"

namespace hare_core
{
    typedef unsigned char	u8;
    typedef signed char		s8;
    typedef unsigned short	u16;
    typedef signed short	s16;
    typedef unsigned int	u32;
    typedef signed int		s32;
    typedef float			f32;
    typedef double			f64;

    typedef std::string     String;
    typedef std::wstring    WString;
    typedef std::vector<String> StringVector;

    typedef TPoint<f32>     PointF;
    typedef TPoint<s32>     PointN;

    typedef TSize<f32>      SizeF;
    typedef TSize<s32>      SizeN;

    typedef TRect<f32>      RectF;
    typedef TRect<s32>      RectN;
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
