//***************************************************************
//  File:    FMODSoundListener.h
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
#ifndef _FMODSOUNDLISTENER_H_
#define _FMODSOUNDLISTENER_H_

namespace hare
{
    class FMODSoundListener : public SoundListener
    {
    public:
        FMODSoundListener();
        virtual ~FMODSoundListener();

        virtual void update();
    };
}

#endif