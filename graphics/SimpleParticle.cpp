#include "PCH.h"
#include "SimpleParticle.h"
#include "Canvas.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_DYNAMIC_CLASS(ParticleInfo, Object, 0)
	{
		HARE_META(nEmission, int)
		HARE_META(fLifeTime, f32)		

		HARE_META(fParticleLifeMin, f32)			
		HARE_META(fParticleLifeMax, f32)			

		HARE_META(fDirection, f32)			
		HARE_META(fSpread,    f32)			
		HARE_META(bRelative,  bool)		

		HARE_META(fSpeedMin, f32)			
		HARE_META(fSpeedMax, f32)			

		HARE_META(fGravityMin, f32)			
		HARE_META(fGravityMax, f32)			

		HARE_META(fRadialAccelMin, f32)			
		HARE_META(fRadialAccelMax, f32)			

		HARE_META(fTangentialAccelMin, f32)			
		HARE_META(fTangentialAccelMax, f32)			

		HARE_META(fSizeStart, f32)			
		HARE_META(fSizeEnd,   f32)			
		HARE_META(fSizeVar,   f32)			

		HARE_META(fSpinStart, f32)			
		HARE_META(fSpinEnd,   f32)			
		HARE_META(fSpinVar,   f32)		

		HARE_META(clColorStart, u32)		
		HARE_META(clColorEnd,   u32)	

		HARE_META(fColorVar, f32)			
		HARE_META(fAlphaVar, f32)			
	}

	ParticleInfo::ParticleInfo()
		:nEmission(7)
		,fLifeTime(-1)		
		,fParticleLifeMin(0.4f)	
		,fParticleLifeMax(1)		
		,fDirection(0)		
		,fSpread(6)			
		,bRelative(true)		
		,fSpeedMin(0)			
		,fSpeedMax(-4)			
		,fGravityMin(-200)	
		,fGravityMax(0)		
		,fRadialAccelMin(-0.6f)		
		,fRadialAccelMax(-0.6f)			
		,fTangentialAccelMin(0)			
		,fTangentialAccelMax(0)			
		,fSizeStart(0.2f)		
		,fSizeEnd(0.9f)		
		,fSizeVar(0)		
		,fSpinStart(0)		
		,fSpinEnd(0)	
		,fSpinVar(0)	
		,clColorStart(0)
		,clColorEnd(-1)
		,fColorVar(0.2f)			
		,fAlphaVar(0)
	{

	}

	ParticleInfo::~ParticleInfo()
	{

	}

	HARE_IMPLEMENT_DYNAMIC_CLASS(SimpleParticle, Particle, 0)
	{
		HARE_OBJ(particleInfo, ParticleInfo)
		HARE_META(fTx, f32)
		HARE_META(fTy, f32)
	}

	SimpleParticle::SimpleParticle()
		:vPrevLocation(0,0)
		,fTx(0)
		,fTy(0)
		,fEmissionResidue(0)
		,nParticlesAlive(0)
		,fAge(-2.0f)
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
		f32 width = (f32)tex->getWidth();
		f32 height= (f32)tex->getHeight();

		ParticleUnit* particleUnit = particleUnits;

		u32 oldColor = getCanvas()->getColor();

		for (int count = 0; count < nParticlesAlive; ++count){
			getCanvas()->setColor(particleUnit->clColor);
			RectF rect;
			rect.minX = particleUnit->vLocation.x;
			rect.minY = particleUnit->vLocation.y;
			rect.maxX = rect.minX + width * particleUnit->fSize;
			rect.maxY = rect.minY + height* particleUnit->fSize;
			getCanvas()->drawImage(rect, particleMtrl, particleUnit->fSpin * particleUnit->fAge);
			particleUnit++;
		}

		getCanvas()->setColor(oldColor);
	}

	void SimpleParticle::frameMove()
	{
		f32 ang;
		ParticleUnit* pPar;
		Vector2 vAccel, vAccel2;

		f32 fDeltaTime = getTimer().getDeltaTime();
		if(fAge >= 0)
		{
			fAge += fDeltaTime;
			if(fAge >= particleInfo->fLifeTime) fAge = -2.0f;
		}

		pPar = particleUnits;

		for(int i=0; i<nParticlesAlive; i++)
		{
			pPar->fAge += fDeltaTime;

			if(pPar->fAge >= pPar->fTerminalAge)
			{
				nParticlesAlive--;
				*pPar = particleUnits[nParticlesAlive];
				i--;
				continue;
			}

			vAccel = pPar->vLocation - vLocation;
			vAccel.normalize();
			vAccel2 = vAccel;
			vAccel *= pPar->fRadialAccel;

			ang = vAccel2.x;
			vAccel2.x = -vAccel2.y;
			vAccel2.y = ang;

			vAccel2 *= pPar->fTangentialAccel;
			pPar->vVelocity += (vAccel + vAccel2) * fDeltaTime;
			pPar->vVelocity.y += pPar->fGravity * fDeltaTime;

			pPar->vLocation += pPar->vVelocity * fDeltaTime;

			pPar->fSpin += pPar->fSpinDelta * fDeltaTime;
			pPar->fSize += pPar->fSizeDelta * fDeltaTime;
			pPar->clColor += pPar->clColorDelta * fDeltaTime;

			pPar++;
		}

		if(fAge != -2.0f)
		{
			float fParticlesNeeded = particleInfo->nEmission * fDeltaTime + fEmissionResidue;
			int nParticlesCreated = (unsigned int)fParticlesNeeded;
			fEmissionResidue = fParticlesNeeded - nParticlesCreated;

			pPar = &particleUnits[nParticlesAlive];

			for(int i=0; i<nParticlesCreated; i++)
			{
				if(nParticlesAlive >= MAX_PARTICLES) break;

				pPar->fAge = 0.0f;
				pPar->fTerminalAge = MathUtil::rand_float(particleInfo->fParticleLifeMin, particleInfo->fParticleLifeMax);

				pPar->vLocation = vPrevLocation + (vLocation - vPrevLocation) * MathUtil::rand_float(0.0f, 1.0f);
				pPar->vLocation.x += MathUtil::rand_float(-2.0f, 2.0f);
				pPar->vLocation.y += MathUtil::rand_float(-2.0f, 2.0f);

				ang = particleInfo->fDirection - MathUtil::PI / 2.f + MathUtil::rand_float(0, particleInfo->fSpread) - particleInfo->fSpread / 2.0f;
				if(particleInfo->bRelative) ang += (vPrevLocation - vLocation).angle() + MathUtil::PI / 2.f;
				pPar->vVelocity.x = MathUtil::cosf(ang);
				pPar->vVelocity.y = MathUtil::sinf(ang);
				pPar->vVelocity *= MathUtil::rand_float(particleInfo->fSpeedMin, particleInfo->fSpeedMax);

				pPar->fGravity = MathUtil::rand_float(particleInfo->fGravityMin, particleInfo->fGravityMax);
				pPar->fRadialAccel = MathUtil::rand_float(particleInfo->fRadialAccelMin, particleInfo->fRadialAccelMax);
				pPar->fTangentialAccel = MathUtil::rand_float(particleInfo->fTangentialAccelMin, particleInfo->fTangentialAccelMax);

				pPar->fSize = MathUtil::rand_float(particleInfo->fSizeStart, particleInfo->fSizeStart + (particleInfo->fSizeEnd - particleInfo->fSizeStart) * particleInfo->fSizeVar);
				pPar->fSizeDelta = (particleInfo->fSizeEnd - pPar->fSize) / pPar->fTerminalAge;

				pPar->fSpin = MathUtil::rand_float(particleInfo->fSpinStart, particleInfo->fSpinStart + (particleInfo->fSpinEnd - particleInfo->fSpinStart) * particleInfo->fSpinVar);
				pPar->fSpinDelta = (particleInfo->fSpinEnd - pPar->fSpin) / pPar->fTerminalAge;

				Color randColorBegin(particleInfo->clColorStart);
				Color randColorEnd(particleInfo->clColorEnd);

				pPar->clColor.R = MathUtil::rand_float(randColorBegin.R, randColorBegin.R + (randColorEnd.R - randColorBegin.R) * particleInfo->fColorVar);
				pPar->clColor.G = MathUtil::rand_float(randColorBegin.G, randColorBegin.G + (randColorEnd.G - randColorBegin.G) * particleInfo->fColorVar);
				pPar->clColor.B = MathUtil::rand_float(randColorBegin.B, randColorBegin.B + (randColorEnd.B - randColorBegin.B) * particleInfo->fColorVar);
				pPar->clColor.A = MathUtil::rand_float(randColorBegin.A, randColorBegin.A + (randColorEnd.A - randColorBegin.A) * particleInfo->fAlphaVar);

				pPar->clColorDelta.R = (randColorEnd.R - pPar->clColor.R) / pPar->fTerminalAge;
				pPar->clColorDelta.G = (randColorEnd.G - pPar->clColor.G) / pPar->fTerminalAge;
				pPar->clColorDelta.B = (randColorEnd.B - pPar->clColor.B) / pPar->fTerminalAge;
				pPar->clColorDelta.A = (randColorEnd.A - pPar->clColor.A) / pPar->fTerminalAge;

				nParticlesAlive++;
				pPar++;
			}
		}

		vPrevLocation = vLocation;

	}

	void SimpleParticle::fire()
	{
		if(particleInfo->fLifeTime == -1.0f) fAge = -1.0f;
		else fAge = 0.0f;
	}

	void SimpleParticle::fireAt(f32 x, f32 y)
	{
		stop();
		setPosition(x, y);
		fire();
	}

	void SimpleParticle::move(f32 offsetX, f32 offsetY)
	{
		PointF pos = getPosition();
		moveTo(pos.x + offsetX, pos.y + offsetY);
	}

	void SimpleParticle::moveTo(f32 x, f32 y)
	{
		float dx,dy;

		dx = x - vLocation.x;
		dy = y - vLocation.y;

		for(int i = 0; i < nParticlesAlive; i++)
		{
			particleUnits[i].vLocation.x += dx;
			particleUnits[i].vLocation.y += dy;
		}

		vPrevLocation.x = vPrevLocation.x + dx;
		vPrevLocation.y = vPrevLocation.y + dy;

		vLocation.x = x;
		vLocation.y = y;
	}

	void SimpleParticle::pause()
	{
		fAge = -2.0f;
	}

	void SimpleParticle::stop()
	{
		pause();
		nParticlesAlive = 0;
	}

	void SimpleParticle::setPosition(f32 x, f32 y)
	{
		if(fAge == -2.0f) { vPrevLocation.x = x; vPrevLocation.y = y; }
		else { vPrevLocation.x = vLocation.x; vPrevLocation.y = vLocation.y; }
	
		vLocation.x = x;
		vLocation.y = y;
	}

	void SimpleParticle::postLoaded()
	{
		MathUtil::clampMinMax(particleInfo->nEmission, 0, 1000);
		MathUtil::clampMinMax(particleInfo->fLifeTime, 0.f, 10.f);
		MathUtil::clampMinMax(particleInfo->fParticleLifeMin,0.f, 5.f);
		MathUtil::clampMinMax(particleInfo->fParticleLifeMax,0.f, 5.f);		
		MathUtil::clampMinMax(particleInfo->fDirection,0.f, 2 * MathUtil::PI);	
		MathUtil::clampMinMax(particleInfo->fSpread,0.f, 2 * MathUtil::PI);					
		MathUtil::clampMinMax(particleInfo->fSpeedMin,-300.f, 300.f);		
		MathUtil::clampMinMax(particleInfo->fSpeedMax,-300.f, 300.f);		
		MathUtil::clampMinMax(particleInfo->fGravityMin,-900.f, 900.f);
		MathUtil::clampMinMax(particleInfo->fGravityMax,-900.f, 900.f);	
		MathUtil::clampMinMax(particleInfo->fRadialAccelMin,-900.f, 900.f);	
		MathUtil::clampMinMax(particleInfo->fRadialAccelMax,-900.f, 900.f);		
		MathUtil::clampMinMax(particleInfo->fTangentialAccelMin,-900.f, 900.f);			
		MathUtil::clampMinMax(particleInfo->fTangentialAccelMax,-900.f, 900.f);		
		MathUtil::clampMinMax(particleInfo->fSizeStart,1.f, 100.f);	
		MathUtil::clampMinMax(particleInfo->fSizeEnd,1.f, 100.f);	
		MathUtil::clampMinMax(particleInfo->fSizeVar,0.f, 1.f);	
		MathUtil::clampMinMax(particleInfo->fSpinStart,-50.f, 50.f);	
		MathUtil::clampMinMax(particleInfo->fSpinEnd,-50.f, 50.f);	
		MathUtil::clampMinMax(particleInfo->fSpinVar,0.f, 1.f);
		MathUtil::clampMinMax(particleInfo->fColorVar,0.f, 1.f);		
		MathUtil::clampMinMax(particleInfo->fAlphaVar,0.f, 1.f);
	}

}