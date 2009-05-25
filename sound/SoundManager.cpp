//***************************************************************
//  File:    SoundManager.cpp
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
#include "SoundManager.h"




namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(SoundManager, Object, 0)
    {

    }

	SoundManager::SoundManager()
		:soundListener(NULL)
	{

	}

	SoundManager::~SoundManager()
	{

	}

    SoundBuffer* SoundManager::getSoundBuffer(const String& name)
    {
        SoundBuffers::iterator it = soundBuffers.find(name);    
        if (it == soundBuffers.end()){
            return NULL;
        }
        return it->second;
    }

    SoundListener* SoundManager::getSoundListener()
    {
        return soundListener;
    }

    bool SoundManager::registSoundBuffer(SoundBuffer* buffer)
    {
        SoundBuffers::iterator it = soundBuffers.find(buffer->getFileName());
        if (it != soundBuffers.end())
            return false;

        soundBuffers[buffer->getFileName()] = buffer;
        return true;
    }

    bool SoundManager::registSoundObject(SoundObject* object)
    {
        SoundObject::Array::iterator it = std::find(soundObjects.begin(), soundObjects.end(), object);
        if (it != soundObjects.end())
            return false;

        soundObjects.push_back(object);
        return true;
    }

    bool SoundManager::unregistSoundBuffer(SoundBuffer* buffer)
    {
        SoundBuffers::iterator it = soundBuffers.find(buffer->getFileName());
        if (it != soundBuffers.end()){
            soundBuffers.erase(it);
            return true;
        }

        return false;
    }

    bool SoundManager::unregistSoundObject(SoundObject* object)
    {
        SoundObject::Array::iterator it = std::find(soundObjects.begin(), soundObjects.end(), object);
        if (it != soundObjects.end()){
            soundObjects.erase(it);
            return true;
        }
            
        return false;
    }

}

