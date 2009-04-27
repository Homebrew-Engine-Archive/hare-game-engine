#ifndef SIMPLESPRITE
#define SIMPLESPRITE

#include "Sprite.h"
#include "Quad.h"
#include "Shader.h"
#include "Particle.h"

#include "GraphicsPrerequisites.h"

namespace hare
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

		void move(float x, float y);
	protected:
		Quad			 quad;
		Shader*		     shader;
		Particle::Ptr    particle;

		//renderToTex test
		Texture*     _tex;
		TextureMtrl::Ptr _texMtrl;
	};
}

#endif
