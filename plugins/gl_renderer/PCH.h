#ifndef _PCH_H_
#define _PCH_H_

#include "core/Core.h"
#include "graphics/Graphics.h"

#if HARE_PLATFORM == HARE_PLATFORM_PSP
#	include <GLES/egl.h>
#else
#	include <GL/glew.h>
#	if HARE_PLATFORM == HARE_PLATFORM_WIN32
#		include <GL/wglew.h>
#	elif HARE_PLATFORM == HARE_PLATFORM_LINUX
#       include <X11/Xlib.h>
#       include <X11/Xutil.h>
#       include <X11/Xlibint.h>
#       include <X11/extensions/render.h>
#       include <X11/extensions/Xrender.h>
#       include <X11/extensions/Xrandr.h>
#		include <GL/glxew.h>
#	endif
#endif

using namespace hare;


#endif
