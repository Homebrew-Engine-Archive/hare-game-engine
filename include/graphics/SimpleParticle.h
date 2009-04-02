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

		s32			nEmission;
		f32			fLifeTime;

		f32			fParticleLifeMin;
		f32			fParticleLifeMax;

		f32			fDirection;
		f32			fSpread;
		bool		bRelative;

		f32			fSpeedMin;
		f32			fSpeedMax;

		f32			fGravityMin;
		f32			fGravityMax;

		f32			fRadialAccelMin;
		f32			fRadialAccelMax;

		f32			fTangentialAccelMin;
		f32			fTangentialAccelMax;

		f32			fSizeStart;
		f32			fSizeEnd;
		f32			fSizeVar;

		f32			fSpinStart;
		f32			fSpinEnd;
		f32			fSpinVar;

		u32			clColorStart;
		u32			clColorEnd;

		f32			fColorVar;
		f32			fAlphaVar;
	};


	class GRAPHICS_API SimpleParticle : public Particle
	{
		HARE_DECLARE_DYNAMIC_CLASS(SimpleParticle)
	public:
		struct ParticleUnit
		{
			Vector2   vLocation;
			Vector2   vVelocity;

			f32	  fGravity;
			f32	  fRadialAccel;
			f32	  fTangentialAccel;

			f32	  fSpin;
			f32	  fSpinDelta;

			f32	  fSize;
			f32	  fSizeDelta;

			Color clColor;
			Color clColorDelta;

			f32	  fAge;
			f32	  fTerminalAge;
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
		f32		fAge;
		f32		fEmissionResidue;

		Vector2	vPrevLocation;

		f32		fTx, fTy;

		int		nParticlesAlive;

		ParticleInfo::Ptr particleInfo;
		ParticleUnit particleUnits[MAX_PARTICLES];

	};
}


#endif