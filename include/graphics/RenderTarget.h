#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "GraphicsPrerequisites.h"

namespace hare
{
	class GRAPHICS_API RenderTarget
	{
	public:
		RenderTarget();
		virtual ~RenderTarget();

		virtual void active() = 0;
		virtual void inactive() = 0;
	};
}

#endif
