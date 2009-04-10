#ifndef _PCH_H_
#define _PCH_H_

#include <vector>
#include <list>
#include <map>
#include <xutility>

using namespace std;

#include "core/Core.h"

using namespace hare_core;

#include "graphics/Graphics.h"

using namespace hare_graphics;

#include "GLRenderPrerequisites.h"

#include <GL/glx.h>
#include <GL/gl.h>
#include "GL/glew.h"
#include "GL/glxew.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#include "GL/wglew.h"
#endif


#endif