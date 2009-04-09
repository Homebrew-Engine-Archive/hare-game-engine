#ifndef _GRAPHICS_PREREQUISITES_H_
#define _GRAPHICS_PREREQUISITES_H_

#include "core/Core.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   if defined(HARE_STATIC_LIB)
#   	define GRAPHICS_API
#   else
#       if defined(GRAPHICS_EXPORTS)
#           define GRAPHICS_API __declspec(dllexport)
#       else
#           define GRAPHICS_API __declspec(dllimport)
#       endif
#   endif
#endif
#if HARE_PLATFORM == HARE_PLATFORM_LINUX || HARE_PLATFORM == HARE_PLATFORM_APPLE || HARE_PLATFORM == HARE_PLATFORM_PSP
#   if defined(HARE_GCC_VISIBILITY)
#       define GRAPHICS_API  __attribute__ ((visibility("default")))
#   else
#       define GRAPHICS_API
#   endif
#endif

#endif // _PREREQUISITES_H_
