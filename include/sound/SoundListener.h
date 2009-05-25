//***************************************************************
//  File:    SoundListener.h
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
#ifndef _SOUND_LISTENER_H_
#define _SOUND_LISTENER_H_

#include "SoundPrerequisites.h"

namespace hare
{
    class SOUND_API SoundListener
	{
	protected:
		Vector2		velocity;
		Vector2		position;
		float   	gain;
		
	public:
		SoundListener();
		virtual ~SoundListener();
		
        virtual void update() = 0;

        void setPosition(const Vector2& position);

        void setVelocity(const Vector2& velocity);

        void setGain(float gain);

		Vector2 getPosition() const;

        Vector2 getVelocity() const;

        float getGain() const;

	};
}

#endif
