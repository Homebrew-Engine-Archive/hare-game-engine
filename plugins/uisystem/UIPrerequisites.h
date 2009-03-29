#ifndef _UI_PREREQUISITES_H_
#define _UI_PREREQUISITES_H_

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   if defined(HARE_STATIC_LIB)
#   	define UI_API
#   else
#       if defined(UI_EXPORTS)
#           define UI_API __declspec(dllexport)
#       else
#           define UI_API __declspec(dllimport)
#       endif
#   endif
#endif
#if HARE_PLATFORM == HARE_PLATFORM_LINUX || HARE_PLATFORM == HARE_PLATFORM_APPLE
#   if defined(HARE_GCC_VISIBILITY)
#       define UI_API  __attribute__ ((visibility("default")))
#   else
#       define UI_API
#   endif
#endif

#endif // _UI_PREREQUISITES_H_