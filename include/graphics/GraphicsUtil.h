#ifndef _GRAPHICSUTIL_H_
#define _GRAPHICSUTIL_H_

#include "GraphicsPrerequisites.h"
#include "HareGraphicsType.h"

namespace hare
{
	class GRAPHICS_API GraphicsUtil
	{
	public:
		static uint32 getPixelFormatBytes(HarePixelFormat hpf);
	};

}
#endif
