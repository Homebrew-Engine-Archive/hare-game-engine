#ifndef _SIMPLEPARTICLE_H_
#define _SIMPLEPARTICLE_H_

#include "GraphicsPrerequisites.h"
#include "Particle.h"
#include "HareGraphicsType.h"

#define MAX_PARTICLES 500

namespace hare_graphics
{

	class ParticleInfo : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(ParticleInfo)
	public:
		ParticleInfo();
		virtual ~ParticleInfo();

		s32			emission;
		f32			lifeTime;

		f32			particleLifeMin;
		f32			particleLifeMax;

		f32			direction;
		f32			spread;
		bool		relative;

		f32			speedMin;
		f32			speedMax;

		f32			gravityMin;
		f32			gravityMax;

		f32			radialAccelMin;
		f32			radialAccelMax;

		f32			tangentialAccelMin;
		f32			tangentialAccelMax;

		f32			sizeStart;
		f32			sizeEnd;
		f32			sizeVar;

		f32			spinStart;
		f32			spinEnd;
		f32			spinVar;

		u32			colorStart;
		u32			colorEnd;

		f32			colorVar;
		f32			alphaVar;
	};


	class GRAPHICS_API SimpleParticle : public Particle
	{
		HARE_DECLARE_DYNAMIC_CLASS(SimpleParticle)
	public:
		struct ParticleUnit
		{
			Vector2   location;
			Vector2   velocity;

			f32	  gravity;
			f32	  radialAccel;
			f32	  tangentialAccel;

			f32	  spin;
			f32	  spinDelta;

			f32	  size;
			f32	  sizeDelta;

			Color color;
			Color colorDelta;

			f32	  age;
			f32	  terminalAge;
		};

	public:
		SimpleParticle();
		virtual ~SimpleParticle();

		virtual void render();
		virtual void frameMove();
		virtual void fire();
		virtual void fireAt(f32 x, f32 y);
		virtual void move(f32 offsetX, f32 offsetY);
		virtual void moveTo(f32 x, f32 y);
		virtual void pause();
		virtual void stop();
		virtual void setPosition(f32 x, f32 y);

		virtual void postLoaded();
		virtual void postEdited(Attribute* attr);
	protected:
		f32		age;
		f32		emissionResidue;

		Vector2	prevLocation;

		f32		tx, ty;

		int		particlesAlive;

		ParticleInfo::Ptr particleInfo;
		ParticleUnit particleUnits[MAX_PARTICLES];

	};
}

#endif
