#include "PCH.h"
#include "SimpleParticle.h"
#include "Canvas.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_DYNAMIC_CLASS(ParticleInfo, Object, 0)
	{
		HARE_META(emission, int32)
		HARE_META(lifeTime, float)		

		HARE_META(particleLifeMin, float)			
		HARE_META(particleLifeMax, float)			

		HARE_META(direction, float)			
		HARE_META(spread,    float)			
		HARE_META(relative,  bool)		

		HARE_META(speedMin, float)			
		HARE_META(speedMax, float)			

		HARE_META(gravityMin, float)			
		HARE_META(gravityMax, float)			

		HARE_META(radialAccelMin, float)			
		HARE_META(radialAccelMax, float)			

		HARE_META(tangentialAccelMin, float)			
		HARE_META(tangentialAccelMax, float)			

		HARE_META(sizeStart, float)			
		HARE_META(sizeEnd,   float)			
		HARE_META(sizeVar,   float)			

		HARE_META(spinStart, float)			
		HARE_META(spinEnd,   float)			
		HARE_META(spinVar,   float)		

		HARE_META_F(colorStart, uint32, propColor)		
		HARE_META_F(colorEnd,   uint32, propColor)	

		HARE_META(colorVar, float)			
		HARE_META(alphaVar, float)			
	}

	ParticleInfo::ParticleInfo()
		:emission(7)
		,lifeTime(-1)		
		,particleLifeMin(0.4f)	
		,particleLifeMax(1)		
		,direction(0)		
		,spread(6)			
		,relative(true)		
		,speedMin(0)			
		,speedMax(-4)			
		,gravityMin(-200)	
		,gravityMax(0)		
		,radialAccelMin(-0.6f)		
		,radialAccelMax(-0.6f)			
		,tangentialAccelMin(0)			
		,tangentialAccelMax(0)			
		,sizeStart(0.2f)		
		,sizeEnd(0.9f)		
		,sizeVar(0)		
		,spinStart(0)		
		,spinEnd(0)	
		,spinVar(0)	
		,colorStart(0)
		,colorEnd(-1)
		,colorVar(0.2f)			
		,alphaVar(0)
	{
	}

	ParticleInfo::~ParticleInfo()
	{
	}

    void ParticleInfo::postLoaded()
    {
        MathUtil::clampMinMax(emission, 0, 1000);
        MathUtil::clampMax(lifeTime, 10.f);
        MathUtil::clampMinMax(particleLifeMin,0.f, 5.f);
        MathUtil::clampMinMax(particleLifeMax,0.f, 5.f);		
        MathUtil::clampMinMax(direction,0.f, 2 * MathUtil::PI);	
        MathUtil::clampMinMax(spread,0.f, 2 * MathUtil::PI);					
        MathUtil::clampMinMax(speedMin,-300.f, 300.f);		
        MathUtil::clampMinMax(speedMax,-300.f, 300.f);		
        MathUtil::clampMinMax(gravityMin,-900.f, 900.f);
        MathUtil::clampMinMax(gravityMax,-900.f, 900.f);	
        MathUtil::clampMinMax(radialAccelMin,-900.f, 900.f);	
        MathUtil::clampMinMax(radialAccelMax,-900.f, 900.f);		
        MathUtil::clampMinMax(tangentialAccelMin,-900.f, 900.f);			
        MathUtil::clampMinMax(tangentialAccelMax,-900.f, 900.f);		
        MathUtil::clampMinMax(sizeStart,1.f, 100.f);	
        MathUtil::clampMinMax(sizeEnd,1.f, 100.f);	
        MathUtil::clampMinMax(sizeVar,0.f, 1.f);	
        MathUtil::clampMinMax(spinStart,-50.f, 50.f);	
        MathUtil::clampMinMax(spinEnd,-50.f, 50.f);	
        MathUtil::clampMinMax(spinVar,0.f, 1.f);
        MathUtil::clampMinMax(colorVar,0.f, 1.f);		
        MathUtil::clampMinMax(alphaVar,0.f, 1.f);
    }

    void ParticleInfo::postEdited(Attribute* attr)
    {
        postLoaded();
    }

	HARE_IMPLEMENT_DYNAMIC_CLASS(SimpleParticle, Particle, 0)
	{
		HARE_OBJ(particleInfo, ParticleInfo)
		HARE_META(tx, float)
		HARE_META(ty, float)
	}

	SimpleParticle::SimpleParticle()
		:age(-2.0f)
        ,emissionResidue(0)
        ,prevLocation(0,0)
		,tx(0)
		,ty(0)
		,particlesAlive(0)
	{
		particleInfo = new ParticleInfo;
	}

	SimpleParticle::~SimpleParticle()
	{

	}

	void SimpleParticle::render()
	{
		TextureMtrl* texMtrl = particleMtrl->getTextureMtrl();
		Texture* tex = texMtrl->getTexture();
		float width = (float)tex->getWidth();
		float height= (float)tex->getHeight();

		ParticleUnit* particleUnit = particleUnits;

		uint32 oldColor = getCanvas()->getColor();
		RectF rect;
		for (int count = 0; count < particlesAlive; ++count){
			getCanvas()->setColor(particleUnit->color);
			rect.minX = particleUnit->location.x;
			rect.minY = particleUnit->location.y;
			rect.maxX = rect.minX + width * particleUnit->size;
			rect.maxY = rect.minY + height* particleUnit->size;
			getCanvas()->drawImage(rect, particleMtrl, particleUnit->spin * particleUnit->age);
			particleUnit++;
		}

		getCanvas()->setColor(oldColor);
	}

	void SimpleParticle::frameMove()
	{
		float ang;
		ParticleUnit* pPar;
		Vector2 vAccel, vAccel2;

		float fDeltaTime = getTimer().getDeltaTime();
		if(age >= 0)
		{
			age += fDeltaTime;
			if(age >= particleInfo->lifeTime) age = -2.0f;
		}

		pPar = particleUnits;

		for(int i=0; i<particlesAlive; i++)
		{
			pPar->age += fDeltaTime;

			if(pPar->age >= pPar->terminalAge)
			{
				particlesAlive--;
				*pPar = particleUnits[particlesAlive];
				i--;
				continue;
			}

			vAccel = pPar->location - location;
			vAccel.normalize();
			vAccel2 = vAccel;
			vAccel *= pPar->radialAccel;

			ang = vAccel2.x;
			vAccel2.x = -vAccel2.y;
			vAccel2.y = ang;

			vAccel2 *= pPar->tangentialAccel;
			pPar->velocity += (vAccel + vAccel2) * fDeltaTime;
			pPar->velocity.y += pPar->gravity * fDeltaTime;

			pPar->location += pPar->velocity * fDeltaTime;

			pPar->spin += pPar->spinDelta * fDeltaTime;
			pPar->size += pPar->sizeDelta * fDeltaTime;
			pPar->color += pPar->colorDelta * fDeltaTime;

			pPar++;
		}

		if(age != -2.0f)
		{
			float fParticlesNeeded = particleInfo->emission * fDeltaTime + emissionResidue;
			int nParticlesCreated = (unsigned int)fParticlesNeeded;
			emissionResidue = fParticlesNeeded - nParticlesCreated;

			pPar = &particleUnits[particlesAlive];

			for(int i=0; i<nParticlesCreated; i++)
			{
				if(particlesAlive >= MAX_PARTICLES) break;

				pPar->age = 0.0f;
				pPar->terminalAge = MathUtil::rand_float(particleInfo->particleLifeMin, particleInfo->particleLifeMax);

				pPar->location = prevLocation + (location - prevLocation) * MathUtil::rand_float(0.0f, 1.0f);
				pPar->location.x += MathUtil::rand_float(-2.0f, 2.0f);
				pPar->location.y += MathUtil::rand_float(-2.0f, 2.0f);

				ang = particleInfo->direction - MathUtil::PI / 2.f + MathUtil::rand_float(0, particleInfo->spread) - particleInfo->spread / 2.0f;
				if(particleInfo->relative) ang += (prevLocation - location).angle() + MathUtil::PI / 2.f;
				pPar->velocity.x = MathUtil::cosf(ang);
				pPar->velocity.y = MathUtil::sinf(ang);
				pPar->velocity *= MathUtil::rand_float(particleInfo->speedMin, particleInfo->speedMax);

				pPar->gravity = MathUtil::rand_float(particleInfo->gravityMin, particleInfo->gravityMax);
				pPar->radialAccel = MathUtil::rand_float(particleInfo->radialAccelMin, particleInfo->radialAccelMax);
				pPar->tangentialAccel = MathUtil::rand_float(particleInfo->tangentialAccelMin, particleInfo->tangentialAccelMax);

				pPar->size = MathUtil::rand_float(particleInfo->sizeStart, particleInfo->sizeStart + (particleInfo->sizeEnd - particleInfo->sizeStart) * particleInfo->sizeVar);
				pPar->sizeDelta = (particleInfo->sizeEnd - pPar->size) / pPar->terminalAge;

				pPar->spin = MathUtil::rand_float(particleInfo->spinStart, particleInfo->spinStart + (particleInfo->spinEnd - particleInfo->spinStart) * particleInfo->spinVar);
				pPar->spinDelta = (particleInfo->spinEnd - pPar->spin) / pPar->terminalAge;

				Color randColorBegin(particleInfo->colorStart);
				Color randColorEnd(particleInfo->colorEnd);

				pPar->color.R = MathUtil::rand_float(randColorBegin.R, randColorBegin.R + (randColorEnd.R - randColorBegin.R) * particleInfo->colorVar);
				pPar->color.G = MathUtil::rand_float(randColorBegin.G, randColorBegin.G + (randColorEnd.G - randColorBegin.G) * particleInfo->colorVar);
				pPar->color.B = MathUtil::rand_float(randColorBegin.B, randColorBegin.B + (randColorEnd.B - randColorBegin.B) * particleInfo->colorVar);
				pPar->color.A = MathUtil::rand_float(randColorBegin.A, randColorBegin.A + (randColorEnd.A - randColorBegin.A) * particleInfo->alphaVar);

				pPar->colorDelta.R = (randColorEnd.R - pPar->color.R) / pPar->terminalAge;
				pPar->colorDelta.G = (randColorEnd.G - pPar->color.G) / pPar->terminalAge;
				pPar->colorDelta.B = (randColorEnd.B - pPar->color.B) / pPar->terminalAge;
				pPar->colorDelta.A = (randColorEnd.A - pPar->color.A) / pPar->terminalAge;

				particlesAlive++;
				pPar++;
			}
		}

		prevLocation = location;

	}

	void SimpleParticle::fire()
	{
		if(particleInfo->lifeTime == -1.0f) age = -1.0f;
		else age = 0.0f;
	}

	void SimpleParticle::fireAt(float x, float y)
	{
		stop();
		setPosition(x, y);
		fire();
	}

	void SimpleParticle::move(float offsetX, float offsetY)
	{
		PointF pos = getPosition();
		moveTo(pos.x + offsetX, pos.y + offsetY);
	}

	void SimpleParticle::moveTo(float x, float y)
	{
        if (age == -2.0f) { prevLocation.x = x; prevLocation.y = y; }
        else { prevLocation.x = location.x; prevLocation.y = location.y; }

        location.x = x;
        location.y = y;
	}

	void SimpleParticle::pause()
	{
		age = -2.0f;
	}

	void SimpleParticle::stop()
	{
		pause();
		particlesAlive = 0;
	}

	void SimpleParticle::setPosition(float x, float y)
	{
        float dx,dy;

        dx = x - location.x;
        dy = y - location.y;

        for(int i = 0; i < particlesAlive; i++)
        {
            particleUnits[i].location.x += dx;
            particleUnits[i].location.y += dy;
        }

        prevLocation.x = prevLocation.x + dx;
        prevLocation.y = prevLocation.y + dy;

        location.x = x;
        location.y = y;
	}

	void SimpleParticle::postLoaded()
	{
	}

	void SimpleParticle::postEdited(Attribute* attr)
	{
        stop();
        fire();
	}
}
