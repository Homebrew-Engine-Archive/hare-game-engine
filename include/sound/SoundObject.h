//***************************************************************
//  File:    SoundObject.h
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
#ifndef	_SOUND_OBJECT_H_
#define _SOUND_OBJECT_H_

#include "SoundPrerequisites.h"
#include "SoundData.h"

namespace hare
{
    class SOUND_API SoundObject : public Object
	{
        HARE_DECLARE_ABSTRACT_CLASS(SoundObject)
	public:
        SoundObject();
        virtual ~SoundObject();
	
		virtual void play() = 0;
		virtual void stop() = 0;
		virtual void pause()= 0;
		virtual void resume()=0;
        virtual void setLopping(bool state) = 0;
        virtual void update() = 0;
		virtual bool getPlaying() = 0;

		void setSoundData(SoundData* data);
        void setPosition(const Vector2& position);
        void setDirection(const Vector2& direction);
        void setVelocity(const Vector2& velocity);
        void setVolume(float val);
     
        SoundData* getSoundData();
		const Vector2& getPosition();
		const Vector2& getDirection();
		const Vector2& getVelocity();
        float getVolume();
        bool  getLooping() const;
		float getPrecomputedDistanceSquared();

    protected:
        bool                looping;
        Vector2	            position;
        Vector2	            direction;
        Vector2	            velocity;
        float               volume;
        float               precomputedDistanceSquared;
        SoundData::Ptr      soundData;

	};

}

#endif
