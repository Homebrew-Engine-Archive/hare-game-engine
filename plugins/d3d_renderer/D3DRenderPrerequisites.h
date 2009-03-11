#ifndef _D3DRENDER_PREREQUISITES_H_
#define _D3DRENDER_PREREQUISITES_H_

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   if defined(HARE_STATIC_LIB)
#   	define D3DRENDER_API
#   else
#       if defined(D3DRENDER_EXPORTS)
#           define D3DRENDER_API __declspec(dllexport)
#       else
#           define D3DRENDER_API __declspec(dllimport)
#       endif
#   endif
#endif
#if HARE_PLATFORM == HARE_PLATFORM_LINUX || HARE_PLATFORM == HARE_PLATFORM_APPLE
#   if defined(HARE_GCC_VISIBILITY)
#       define D3DRENDER_API  __attribute__ ((visibility("default")))
#   else
#       define D3DRENDER_API
#   endif
#endif

#endif // _PREREQUISITES_H_
