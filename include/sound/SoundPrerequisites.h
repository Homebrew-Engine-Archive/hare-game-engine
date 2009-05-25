//***************************************************************
//  File:    SoundPrerequisites.h
//  Date:    5/22/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _SOUND_PREREQUISITES_H_
#define _SOUND_PREREQUISITES_H_

#include "core/Core.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   if defined(HARE_STATIC_LIB)
#   	define SOUND_API
#   else
#       if defined(SOUND_EXPORTS)
#           define SOUND_API __declspec(dllexport)
#       else
#           define SOUND_API __declspec(dllimport)
#       endif
#   endif
#elif HARE_PLATFORM == HARE_PLATFORM_LINUX || HARE_PLATFORM == HARE_PLATFORM_APPLE || HARE_PLATFORM == HARE_PLATFORM_PSP
#   if defined(HARE_GCC_VISIBILITY)
#       define SOUND_API  __attribute__ ((visibility("default")))
#   else
#       define SOUND_API
#   endif
#endif

#endif // _PREREQUISITES_H_