#ifndef _PARTICE_H_
#define _PARTICE_H_

#include "GraphicsPrerequisites.h"
#include "Shader.h"
namespace hare_graphics
{
	class GRAPHICS_API Particle : public Object
	{
		HARE_DECLARE_ABSTRACT_CLASS(Particle)
	public:
		Particle();
		virtual ~Particle();

		virtual void render() = 0;
		virtual void frameMove() = 0;
		virtual void fire() = 0;
		virtual void fireAt(float x, float y) = 0;
		virtual void move(float offsetX, float offsetY) = 0;
		virtual void moveTo(float x, float y) = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;

		virtual PointF getPosition();
		virtual void   setPosition(float x, float y);
		void   setMaterial(Material* mtrl);

	protected:
		Vector2       location;
		Material::Ptr particleMtrl;
	};
}

#endif
