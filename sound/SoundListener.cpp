//***************************************************************
//  File:    SoundListener.cpp
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
#include "SoundListener.h"

namespace hare
{
	SoundListener::SoundListener()
		:position(0,0)
        ,velocity(0,0)
        ,gain(1.0f)
	{
		
	}

    SoundListener::~SoundListener()
	{

	}
	
	void SoundListener::setVelocity(const Vector2& velocity)
	{
		this->velocity = velocity;
	}

    void SoundListener::setPosition(const Vector2& position)
	{
		this->position = position;
	}

    void SoundListener::setGain(float gain)
    {
        this->gain = gain;
    }

	Vector2 SoundListener::getVelocity() const
	{
		return velocity;
	}

    Vector2 SoundListener::getPosition() const
	{
		return position;
	}

    float SoundListener::getGain() const
	{
		return gain;
	}
}
