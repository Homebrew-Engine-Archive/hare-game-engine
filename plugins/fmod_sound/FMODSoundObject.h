//***************************************************************
//  File:    FMODSoundObject.h
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
#ifndef _FMODSOUNDOBJECT_H_
#define _FMODSOUNDOBJECT_H_

namespace hare
{
    class FMODSoundObject : public SoundObject
    {
        HARE_DECLARE_DYNAMIC_CLASS(FMODSoundObject)
    public:
        FMODSoundObject();
        virtual ~FMODSoundObject();

        virtual void play();
        virtual void stop();
        virtual void pause();
        virtual void resume();
        virtual void setLopping(bool state);
        virtual void update();


    protected:
        FMOD::Channel   *pChannel;

    };
}

#endif