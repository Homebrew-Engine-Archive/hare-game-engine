#ifndef SIMPLESPRITE
#define SIMPLESPRITE

#include "Sprite.h"
#include "Quad.h"
#include "Shader.h"

#include "GraphicsPrerequisites.h"

namespace hare_graphics
{
	class TextureMtrl;
	class Texture;
	class AnimModUnit;

	class GRAPHICS_API SimpleSprite : public Sprite
	{
	public:
		SimpleSprite();
		virtual ~SimpleSprite();

		virtual void beginScene();

		virtual void renderScene();

		virtual void endScene();

		void loadFromImage(const String& filename);

		void move(int x, int y);
	protected:
		Quad			 quad;
		Shader*		     shader;

	};
}

#endif