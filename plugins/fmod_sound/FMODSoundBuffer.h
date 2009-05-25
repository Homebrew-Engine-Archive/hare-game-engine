//***************************************************************
//  File:    FMODSoundBuffer.h
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
#ifndef _FMODSOUNDBUFFER_H_
#define _FMODSOUNDBUFFER_H_

namespace hare
{
    class FMODSoundBuffer : public SoundBuffer
    {
        HARE_DECLARE_DYNAMIC_CLASS(FMODSoundBuffer)
    public:
        FMODSoundBuffer();
        FMODSoundBuffer(const String& name);
        virtual ~FMODSoundBuffer();

        virtual void load(bool bStream = false);

        virtual void unload();

        virtual void update();

        FMOD::Sound* getSound();

    protected:
        FMOD::Sound *pSound;
        DataHolder  buffer;

    };
}


#endif