//***************************************************************
//  File:    SoundObject.cpp
//  Date:    5/22/2009
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
#include "SoundBuffer.h"
#include "SoundObject.h"
#include "SoundApp.h"
#include "SoundManager.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(SoundObject, Object, 0)
    {

    }

	SoundObject::SoundObject()
		:direction(0,0)
        ,velocity(0,0)
        ,position(0,0)
        ,volume(100)
        ,precomputedDistanceSquared(0)
        ,looping(false)
	{

	}

    SoundObject::~SoundObject()
	{

	}

	void SoundObject::setSoundData(SoundData* data)
	{
		soundData = data;
	}

    void SoundObject::setDirection(const Vector2& param)
    {
        direction = param;
    }

    void SoundObject::setVelocity(const Vector2& param)
    {
        velocity = param;
    }

    void SoundObject::setPosition(const Vector2& param)
    {
        position = param;
    }

    void SoundObject::setVolume(float val)
    {
        volume = val;
    }


	SoundData* SoundObject::getSoundData()
	{
		return soundData;
	}

	const Vector2& SoundObject::getPosition()
	{
		return position;
	}

	const Vector2& SoundObject::getDirection()
	{
		return direction;
	}

	const Vector2& SoundObject::getVelocity()
	{
		return velocity;
	}

    bool SoundObject::getLooping() const
    {
        return looping;
    }
	
    float SoundObject::getPrecomputedDistanceSquared()
	{
		return precomputedDistanceSquared;
	}

    float SoundObject::getVolume()
    {
        return volume;
    }

}

