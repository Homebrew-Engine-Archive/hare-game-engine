#ifndef _UI_PREREQUISITES_H_
#define _UI_PREREQUISITES_H_

#include "core/Core.h"
using namespace hare;

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
#elif HARE_PLATFORM == HARE_PLATFORM_LINUX || HARE_PLATFORM == HARE_PLATFORM_APPLE || HARE_PLATFORM == HARE_PLATFORM_PSP
#   if defined(HARE_GCC_VISIBILITY)
#       define UI_API  __attribute__ ((visibility("default")))
#   else
#       define UI_API
#   endif
#endif

namespace hare
{
    class GUISystem;
    class Window;
    class Theme;
    class Sizer;
    class BoxSizer;
    class SizerItem;
    class SizerItemWindow;
    class SizerItemSizer;
    class SizerItemSpacer;
    class SizerSpacer;
    class Event;
    class MouseEvent;
    class EventHandler;
    struct EventTable;
    struct EventTableEntryBase;
    struct EventTableEntry;
    struct DynamicEventTableEntry;

    typedef TRect<float> RectUV;

    typedef Pointer<Window> WindowPtr;
    typedef Pointer<Sizer> SizerPtr;
    typedef Pointer<SizerSpacer> SizerSpacerPtr;
    
}

#include "GUIDefines.h"

#endif // _UI_PREREQUISITES_H_
