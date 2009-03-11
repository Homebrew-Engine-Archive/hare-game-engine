#ifndef RENDERTARGET
#define RENDERTARGET


#include "GraphicsPrerequisites.h"
namespace hare_graphics
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