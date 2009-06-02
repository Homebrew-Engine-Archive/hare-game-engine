#ifndef _CORE_PREREQUISITES_H_
#define _CORE_PREREQUISITES_H_

#include "Platforms.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   if defined(HARE_STATIC_LIB)
#   	define CORE_API
#   else
#       if defined(CORE_EXPORTS)
#           define CORE_API __declspec(dllexport)
#       else
#           define CORE_API __declspec(dllimport)
#       endif
#   endif
#elif HARE_PLATFORM == HARE_PLATFORM_LINUX || HARE_PLATFORM == HARE_PLATFORM_APPLE || HARE_PLATFORM == HARE_PLATFORM_PSP
#   if defined(HARE_GCC_VISIBILITY)
#       define CORE_API  __attribute__ ((visibility("default")))
#   else
#       define CORE_API
#   endif
#endif

#include "StdHeaders.h"
#include "Types.h"
#include "Pointer.h"
#include "Macros.h"

namespace hare
{
    class ReferenceCounted;
    class Object;
    class ClassInfo;
    class StringConverter;
    class StringUtil;
    class XmlVisitor;
    class BinVisitor;
    class AttVisitor;
    class ConfigFile;
    class Exception;
    class CmdLineParser;
    template<typename T> class Singleton;
    class Timer;
    class Mutex;
    class MutexLocker;
    class CriticalSection;
    class CriticalSectionLocker;
    class Semaphore;
    class Condition;
    class Thread;
    class DynamicLibrary;
    class PluginManager;
}

#endif // _PREREQUISITES_H_
