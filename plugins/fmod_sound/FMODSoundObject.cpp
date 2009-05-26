//***************************************************************
//  File:    FMODSoundObject.cpp
//  Date:    5/24/2009
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
#include "FMODSoundObject.h"
#include "FMODSoundBuffer.h"
#include "FMODSoundManager.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(FMODSoundObject, SoundObject, 0)
    {

    }

    FMODSoundObject::FMODSoundObject()
        :pChannel(NULL)
    {

    }

    FMODSoundObject::~FMODSoundObject()
    {

    }

    void FMODSoundObject::play()
    {
        if (!soundData)
            HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "soundBuffer null!", "FMODSoundObject::play"); 

        FMODSoundBuffer* fmodSoundBuffer = (FMODSoundBuffer*)soundData->getSoundBuffer();

        FMOD::Sound* pSound = fmodSoundBuffer->getSound();

        FMOD::System* system = ((FMODSoundManager*)(getSoundApp()->getSoundManager()))->getFMODSystem();

        system->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);

    }

    void FMODSoundObject::stop()
    {
        if (pChannel)
            pChannel->stop();
    }

    void FMODSoundObject::pause()
    {
        if (pChannel)
            pChannel->setPaused(true);
    }

    void FMODSoundObject::resume()
    {
        if (pChannel)
            pChannel->setPaused(false);
    }

    void FMODSoundObject::setLopping(bool state)
    {
        looping = state;
        if (state){
            pChannel->setMode(FMOD_LOOP_NORMAL);
        }else{
            pChannel->setMode(FMOD_LOOP_OFF);
        }
    }

    bool FMODSoundObject::getPlaying()
    {
        bool bPlaying;
        pChannel->isPlaying(&bPlaying);
        return bPlaying;
    }

    void FMODSoundObject::update()
    {
        if (!pChannel)
            return;

        FMOD_VECTOR vel;
        vel.x = velocity.x;
        vel.y = velocity.y;
        vel.z = 0.f;
        FMOD_VECTOR pos;
        pos.x = position.x;
        pos.y = position.y;
        pos.z = 0.f;
        pChannel->set3DAttributes(&pos, &vel);
        pChannel->setVolume(volume);
    }

}