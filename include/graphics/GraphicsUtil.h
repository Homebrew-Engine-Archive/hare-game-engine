#ifndef GRAPHICSUTIL
#define GRAPHICSUTIL

#include "GraphicsPrerequisites.h"
#include "HareGraphicsType.h"

namespace hare_graphics
{
	class GRAPHICS_API GraphicsUtil
	{
	public:
		static u32 getPixelFormatBytes(HarePixelFormat hpf);
	};

}
#endif
