//***************************************************************
//  File:    SoundBuffer.h
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
#ifndef	_SOUNDBUFFER_H_
#define _SOUNDBUFFER_H_

#include "SoundPrerequisites.h"

namespace hare
{
    class SOUND_API SoundBuffer : public Object
	{
        HARE_DECLARE_ABSTRACT_CLASS(SoundBuffer)
	public:
        SoundBuffer();
		SoundBuffer(const String& name);
		virtual ~SoundBuffer();

        virtual void load(bool bStream = false) = 0;

        virtual void unload() = 0;

        virtual void update() = 0;

        virtual void postEdited(Attribute *attr);

        virtual void postLoaded();

        const String& getFileName();

    public:
        void setCone(float inner, float outer);
        void setGain(float gain, float outerGain,
            float min = 0.0f, float max = 1.0f);
        void setDistance(float ref, float max);
        void setPitch(float pitch);
        void setRolloffFactor(float rolloff);

        float getGain() const;
        float getGainOuter() const;
        float getGainMin() const;
        float getGainMax() const;
        float getRolloff() const;
        float getReferenceDistance() const;
        float getMaxDistance() const;
        float getPitch() const;
        float getInnerCone() const;
        float getOuterCone() const;
        
    protected:
        String fileName;

    protected:
        float   innerCone;
        float   outerCone;
        float   minGain;
        float   maxGain;
        float   refDistance;
        float   maxDistance;
        float   pitch;
        float   gain;
        float   outerGain;
        float   rolloff;

	};
}

#endif
