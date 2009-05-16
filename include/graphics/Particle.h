//***************************************************************
//  File:    Particle.h
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
#ifndef _PARTICE_H_
#define _PARTICE_H_

#include "GraphicsPrerequisites.h"
#include "Shader.h"
#include "Sprite.h"

namespace hare
{
	class GRAPHICS_API Particle : public Sprite
	{
		HARE_DECLARE_ABSTRACT_CLASS(Particle)
	public:
		Particle();
		virtual ~Particle();

		virtual void render() = 0;
		virtual void fire() = 0;
		virtual void fireAt(float x, float y) = 0;
		virtual void move(float dx, float dy);
		virtual void moveTo(float x, float y);
		virtual void pause() = 0;
		virtual void stop() = 0;

		virtual const PointF& getPosition();
		virtual void   setPosition(float x, float y);
		void   setMaterial(Material* mtrl);
	protected:
		virtual void frameMove() = 0;

	protected:
		Vector2       location;
		Material::Ptr particleMtrl;
	};
}

#endif
