//***************************************************************
//  File:    ParticleSprite.cpp
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
#include "ParticleSprite.h"
#include "Canvas.h"

namespace hare
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

        static const float size_min = 1.0f / 32.0f;
        static const float size_max = 100.0f / 32.0f;

        MathUtil::clampMinMax(sizeStart, size_min, size_max);
        MathUtil::clampMinMax(sizeEnd, size_min, size_max);
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

	HARE_IMPLEMENT_DYNAMIC_CLASS(ParticleSprite, Particle, 0)
	{
		HARE_OBJ_F(particleInfo, ParticleInfo, propAvoidNull)
	}

	ParticleSprite::ParticleSprite()
		:age(-2.0f)
        ,emissionResidue(0)
        ,prevLocation(0,0)
		,particlesAlive(0)
	{
		particleInfo = new ParticleInfo;
	}

	ParticleSprite::~ParticleSprite()
	{

	}

	void ParticleSprite::render()
	{
		frameMove();

        if (!particleMtrl)
            return;

		TextureMtrl* texMtrl = particleMtrl->getTextureMtrl();
        
        if (!texMtrl)
            return;

		Texture* tex = texMtrl->getTexture();

        if (!tex)
            return;

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

            //Á£×Ó¼Ì³Ð¾«Áé¸Ä 09/05/16 wl
            float origoX = origoPos.x;
            float origoY = origoPos.y;
            if (!bFaceX){
                origoX = -origoX;
            }
            if (!bFaceY){
                origoY = -origoY;
            }
            
            rect.move(origoX, origoY);

			getCanvas()->drawImage(rect, particleMtrl, particleUnit->spin * particleUnit->age);
			particleUnit++;
		}

		getCanvas()->setColor(oldColor);
	}

	void ParticleSprite::frameMove()
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
				pPar->terminalAge = MathUtil::randRange(particleInfo->particleLifeMin, particleInfo->particleLifeMax);

				pPar->location = prevLocation + (location - prevLocation) * MathUtil::randRange(0.0f, 1.0f);
				pPar->location.x += MathUtil::randRange(-2.0f, 2.0f);
				pPar->location.y += MathUtil::randRange(-2.0f, 2.0f);

				ang = particleInfo->direction - MathUtil::PI / 2.f + MathUtil::randRange(0.0f, particleInfo->spread) - particleInfo->spread / 2.0f;
				if(particleInfo->relative) ang += (prevLocation - location).angle() + MathUtil::PI / 2.f;
				pPar->velocity.x = MathUtil::cosf(ang);
				pPar->velocity.y = MathUtil::sinf(ang);
				pPar->velocity *= MathUtil::randRange(particleInfo->speedMin, particleInfo->speedMax);

				pPar->gravity = MathUtil::randRange(particleInfo->gravityMin, particleInfo->gravityMax);
				pPar->radialAccel = MathUtil::randRange(particleInfo->radialAccelMin, particleInfo->radialAccelMax);
				pPar->tangentialAccel = MathUtil::randRange(particleInfo->tangentialAccelMin, particleInfo->tangentialAccelMax);

				pPar->size = MathUtil::randRange(particleInfo->sizeStart, particleInfo->sizeStart + (particleInfo->sizeEnd - particleInfo->sizeStart) * particleInfo->sizeVar);
				pPar->sizeDelta = (particleInfo->sizeEnd - pPar->size) / pPar->terminalAge;

				pPar->spin = MathUtil::randRange(particleInfo->spinStart, particleInfo->spinStart + (particleInfo->spinEnd - particleInfo->spinStart) * particleInfo->spinVar);
				pPar->spinDelta = (particleInfo->spinEnd - pPar->spin) / pPar->terminalAge;

				Color randColorBegin(particleInfo->colorStart);
				Color randColorEnd(particleInfo->colorEnd);

				pPar->color.R = MathUtil::randRange(randColorBegin.R, randColorBegin.R + (randColorEnd.R - randColorBegin.R) * particleInfo->colorVar);
				pPar->color.G = MathUtil::randRange(randColorBegin.G, randColorBegin.G + (randColorEnd.G - randColorBegin.G) * particleInfo->colorVar);
				pPar->color.B = MathUtil::randRange(randColorBegin.B, randColorBegin.B + (randColorEnd.B - randColorBegin.B) * particleInfo->colorVar);
				pPar->color.A = MathUtil::randRange(randColorBegin.A, randColorBegin.A + (randColorEnd.A - randColorBegin.A) * particleInfo->alphaVar);

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

	void ParticleSprite::fire()
	{
		if(particleInfo->lifeTime == -1.0f) age = -1.0f;
		else age = 0.0f;
	}

	void ParticleSprite::fireAt(float x, float y)
	{
		stop();
		setPosition(x, y);
		fire();
	}

	void ParticleSprite::move(float dx, float dy)
	{
		PointF pos = getPosition();
		moveTo(pos.x + dx, pos.y + dy);
	}

	void ParticleSprite::moveTo(float x, float y)
	{
        if (age == -2.0f) { prevLocation.x = x; prevLocation.y = y; }
        else { prevLocation.x = location.x; prevLocation.y = location.y; }

        location.x = x;
        location.y = y;
	}

	void ParticleSprite::pause()
	{
		age = -2.0f;
	}

	void ParticleSprite::stop()
	{
		pause();
		particlesAlive = 0;
	}

	void ParticleSprite::setPosition(float x, float y)
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

	void ParticleSprite::postLoaded()
	{
        stop();
        fire();
	}

	void ParticleSprite::postEdited(Attribute* attr)
	{
        postLoaded();
	}
}
