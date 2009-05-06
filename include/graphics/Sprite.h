#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "GraphicsPrerequisites.h"

namespace hare
{
	class GRAPHICS_API Sprite
	{
	public:
		Sprite();
		virtual ~Sprite();

        virtual void move(float dx, float dy) = 0;

        virtual void moveTo(float x, float y) = 0;

		virtual void beginScene() = 0;

		virtual void renderScene() = 0;

		virtual void endScene() = 0;
	};
}

#endif
