//***************************************************************
//  File:    SimpleParticle.h
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _SIMPLEPARTICLE_H_
#define _SIMPLEPARTICLE_H_

#include "GraphicsPrerequisites.h"
#include "Particle.h"
#include "HareGraphicsType.h"

#define MAX_PARTICLES 500

namespace hare
{

	class ParticleInfo : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(ParticleInfo)
	public:
		ParticleInfo();
		virtual ~ParticleInfo();

        virtual void postLoaded();
        virtual void postEdited(Attribute* attr);

		int32			emission;
		float			lifeTime;

		float			particleLifeMin;
		float			particleLifeMax;

		float			direction;
		float			spread;
		bool		    relative;

		float			speedMin;
		float			speedMax;

		float			gravityMin;
		float			gravityMax;

		float			radialAccelMin;
		float			radialAccelMax;

		float			tangentialAccelMin;
		float			tangentialAccelMax;

		float			sizeStart;
		float			sizeEnd;
		float			sizeVar;

		float			spinStart;
		float			spinEnd;
		float			spinVar;

		uint32			colorStart;
		uint32			colorEnd;

		float			colorVar;
		float			alphaVar;
	};


	class GRAPHICS_API SimpleParticle : public Particle
	{
		HARE_DECLARE_DYNAMIC_CLASS(SimpleParticle)
	public:
		struct ParticleUnit
		{
			Vector2   location;
			Vector2   velocity;

			float	  gravity;
			float	  radialAccel;
			float	  tangentialAccel;

			float	  spin;
			float	  spinDelta;

			float	  size;
			float	  sizeDelta;

			Color     color;
			Color     colorDelta;

			float	  age;
			float	  terminalAge;
		};

	public:
		SimpleParticle();
		virtual ~SimpleParticle();

		virtual void render();
		virtual void fire();
		virtual void fireAt(float x, float y);
		virtual void move(float offsetX, float offsetY);
		virtual void moveTo(float x, float y);
		virtual void pause();
		virtual void stop();
		virtual void setPosition(float x, float y);

		virtual void postLoaded();
		virtual void postEdited(Attribute* attr);

	protected:
		virtual void frameMove();

	protected:
		float		age;
		float		emissionResidue;
		Vector2	    prevLocation;
		int		    particlesAlive;

		ParticleInfo::Ptr particleInfo;
		ParticleUnit particleUnits[MAX_PARTICLES];

	};
}

#endif
