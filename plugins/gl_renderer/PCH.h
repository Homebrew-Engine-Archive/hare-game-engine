#ifndef _PCH_H_
#define _PCH_H_

#include "core/Core.h"
#include "graphics/Graphics.h"
using namespace hare;

#if HARE_PLATFORM == HARE_PLATFORM_PSP
#	include <GLES/egl.h>
#else
#	include <GL/glew.h>
#	if HARE_PLATFORM == HARE_PLATFORM_WIN32
#		include <GL/wglew.h>
#	elif HARE_PLATFORM == HARE_PLATFORM_LINUX
#		include <GL/glxew.h>
#	endif
#endif

#endif
