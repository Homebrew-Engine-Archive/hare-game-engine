//***************************************************************
//  File:    FMODSoundManager.cpp
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
#include "FMODSoundManager.h"
#include "FMODSoundObject.h"
#include "FMODSoundBuffer.h"
#include "FMODSoundListener.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(FMODSoundManager, SoundManager, 0)
    {

    }

    FMODSoundManager::FMODSoundManager()
        :pSystem(NULL)
    {

    }

    FMODSoundManager::~FMODSoundManager()
    {

    }

    void FMODSoundManager::initialize()
    {
        FMOD::System_Create(&pSystem);

        unsigned int version;

        pSystem->getVersion(&version);

        if(version < FMOD_VERSION)
            return;

        pSystem->setSoftwareChannels(100);

        pSystem->init(100, FMOD_INIT_NORMAL, 0);

        pSystem->set3DSettings(1.0f, 0.1f, 1.0f);

        soundListener = new FMODSoundListener;

    }

    void FMODSoundManager::release()
    {
        if(pSystem){
            pSystem->close();

            pSystem->release();

            pSystem = NULL;

            delete soundListener;

            soundListener = NULL;
        }
    }

    void FMODSoundManager::update()
    {
        SoundBuffer::Array::iterator it = soundBuffers.begin();
        for (;it != soundBuffers.end(); ++it){
            (*it)->update();
        }

        SoundObject::Array::iterator ait = soundObjects.begin();
        for (;ait != soundObjects.end(); ++ait){
            (*ait)->update();
        }

        if (soundListener)
            soundListener->update();

        if (pSystem)
            pSystem->update();

    }

    SoundObject* FMODSoundManager::createSoundObject()
    {
        FMODSoundObject::Ptr soundObject = new FMODSoundObject;
        bool ok = registSoundObject(soundObject);
        assert(ok);
        return soundObject;
    }

    SoundBuffer* FMODSoundManager::createSoundBuffer()
    {
        SoundBuffer* soundBuffer = new FMODSoundBuffer;
        bool ok = registSoundBuffer(soundBuffer);
        assert(ok);
        return soundBuffer;
    }

    FMOD::System* FMODSoundManager::getFMODSystem()
    {
        return pSystem;
    }
}