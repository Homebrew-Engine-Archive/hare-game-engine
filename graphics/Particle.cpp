#include "PCH.h"
#include "Particle.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_ABSTRACT_CLASS(Particle, Object, 0)
	{
        HARE_OBJ(particleMtrl, Material)
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

	void Particle::setPosition(f32 x, f32 y)
	{
		location.x = x;
		location.y = y;
	}
}