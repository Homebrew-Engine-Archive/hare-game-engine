//***************************************************************
//  File:    FMODSoundManager.h
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
#ifndef _FMODSOUNDMANAGER_H_
#define _FMODSOUNDMANAGER_H_

namespace hare
{
    class FMODSoundManager : public SoundManager
    {
        HARE_DECLARE_DYNAMIC_CLASS(FMODSoundManager)
    public:
        FMODSoundManager();
        virtual ~FMODSoundManager();

        virtual void initialize();

        virtual void release();

        virtual void update();

        virtual SoundObject* createSoundObject();

        virtual SoundBuffer* createSoundBuffer(const String& name);

        FMOD::System* getFMODSystem();

    protected:
        FMOD::System    *pSystem;

    };
}

#endif