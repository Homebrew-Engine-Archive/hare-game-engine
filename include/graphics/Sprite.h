#ifndef SPRITE
#define SPRITE

#include "GraphicsPrerequisites.h"

namespace hare_graphics
{
	class GRAPHICS_API Sprite
	{
	public:
		Sprite();
		virtual ~Sprite();

		virtual void beginScene() = 0;

		virtual void renderScene() = 0;

		virtual void endScene() = 0;
	};
}

#endif
