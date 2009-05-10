//***************************************************************
//  File:    Particle.cpp
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
#include "PCH.h"
#include "Particle.h"

namespace hare
{
	HARE_IMPLEMENT_ABSTRACT_CLASS(Particle, Object, 0)
	{
        HARE_OBJ_F(particleMtrl, Material, propImport)
    }

	Particle::Particle()
		:location(0,0)
	{

	}

	Particle::~Particle()
	{

	}

	void Particle::setMaterial(Material* mtrl)
	{
		particleMtrl = mtrl;
	}

	PointF Particle::getPosition()
	{
		return PointF(location.x, location.y);
	}

	void Particle::setPosition(float x, float y)
	{
		location.x = x;
		location.y = y;
	}
}
