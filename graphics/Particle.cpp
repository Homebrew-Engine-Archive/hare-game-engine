#include "PCH.h"
#include "Particle.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_ABSTRACT_CLASS(Particle, Object, 0)
	{}

	Particle::Particle()
		:vLocation(0,0)
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
		return PointF(vLocation.x, vLocation.y);
	}

	void Particle::setPosition(f32 x, f32 y)
	{
		vLocation.x = x;
		vLocation.y = y;
	}
}