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
		virtual void fireAt(f32 x, f32 y) = 0;
		virtual void move(f32 offsetX, f32 offsetY) = 0;
		virtual void moveTo(f32 x, f32 y) = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;

		virtual PointF getPosition();
		virtual void   setPosition(f32 x, f32 y);
		void   setMaterial(Material* mtrl);

	protected:
		Vector2       location;
		Material::Ptr particleMtrl;
	};
}


#endif