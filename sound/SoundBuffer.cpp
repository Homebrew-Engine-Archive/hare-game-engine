//***************************************************************
//  File:    Sound3DBuffer.cpp
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
#include "SoundApp.h"
#include "SoundManager.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(SoundBuffer, Object, 0)
    {
        HARE_META(fileName, String)
    }

    SoundBuffer::SoundBuffer()
        :innerCone(360.0f)
        ,outerCone(360.0f)
        ,minGain(0.0f)
        ,maxGain(1.0f)
        ,refDistance(100.0f)
        ,maxDistance(1000.0f)
        ,pitch(1.0f)
        ,gain(1.0)
        ,outerGain(0.0f)
        ,rolloff(1.0f)
    {

    }
	SoundBuffer::SoundBuffer(const String& name)
        :innerCone(360.0f)
        ,outerCone(360.0f)
        ,minGain(0.0f)
        ,maxGain(1.0f)
        ,refDistance(100.0f)
        ,maxDistance(1000.0f)
        ,pitch(1.0f)
        ,gain(1.0)
        ,outerGain(0.0f)
        ,rolloff(1.0f)
	{
		fileName = name;
	}

	SoundBuffer::~SoundBuffer()
	{

	}

    void SoundBuffer::postEdited(Attribute *attr)
    {

    }

    void SoundBuffer::postLoaded()
    {
        getSoundApp()->getSoundManager()->registSoundBuffer(this);
    }

    const String& SoundBuffer::getFileName()
    {
        return fileName;
    }

    void SoundBuffer::setCone(float inner, float outer)
    {
        innerCone = inner;
        outerCone = outer;
    }

    void SoundBuffer::setGain(float gain, float outerGain, float min, float max)
    {
        this->gain = gain;
        this->outerGain = outerGain;
        minGain = min;
        maxGain = max;
    }

    void SoundBuffer::setDistance(float ref, float max)
    {
        refDistance = ref;
        maxDistance = max;
    }

    void SoundBuffer::setPitch(float pitch)
    {
        this->pitch = pitch;
    }

    void SoundBuffer::setRolloffFactor(float rolloff)
    {
        this->rolloff = rolloff;
    }

    float SoundBuffer::getGain() const
    {
        return gain;
    }

    float SoundBuffer::getGainOuter() const
    {
        return outerGain;
    }

    float SoundBuffer::getGainMin() const
    {
        return minGain;
    }

    float SoundBuffer::getGainMax() const
    {
        return maxGain;
    }

    float SoundBuffer::getRolloff() const
    {
        return rolloff;
    }

    float SoundBuffer::getReferenceDistance() const
    {
        return refDistance;
    }

    float SoundBuffer::getMaxDistance() const
    {
        return maxDistance;
    }

    float SoundBuffer::getPitch() const
    {
        return pitch;
    }

    float SoundBuffer::getInnerCone() const
    {
        return innerCone;
    }

    float SoundBuffer::getOuterCone() const
    {
        return outerCone;
    }
}
