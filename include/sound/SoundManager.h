//***************************************************************
//  File:    SoundManager.h
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
#ifndef	_SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include "SoundPrerequisites.h"
#include "SoundBuffer.h"
#include "SoundObject.h"

namespace hare
{
    class SoundListener;

    class SOUND_API SoundManager : public Object
	{
        HARE_DECLARE_ABSTRACT_CLASS(SoundManager)
	public:
        typedef std::map<String, SoundBuffer::Ptr> SoundBuffers;
	
    protected:
        SoundObject::Array  soundObjects;
		SoundBuffers        soundBuffers;
        SoundListener*      soundListener;
		
	public:
		SoundManager();
		virtual ~SoundManager();

        virtual void initialize() = 0;

        virtual void release() = 0;

        virtual void update() = 0;

        virtual SoundObject* createSoundObject() = 0;
		
		virtual SoundBuffer* createSoundBuffer(const String& name) = 0;

		virtual SoundBuffer* getSoundBuffer(const String& name);

        virtual SoundListener* getSoundListener();

        bool registSoundBuffer(SoundBuffer* buffer);
         
        bool registSoundObject(SoundObject* object);

        bool unregistSoundBuffer(SoundBuffer* buffer);

        bool unregistSoundObject(SoundObject* object);
    };
}

#endif
