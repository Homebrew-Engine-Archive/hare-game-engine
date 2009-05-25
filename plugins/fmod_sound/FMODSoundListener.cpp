//***************************************************************
//  File:    FMODSoundListener.cpp
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
#include "FMODSoundBuffer.h"
#include "FMODSoundListener.h"
#include "FMODSoundManager.h"

namespace hare
{
    FMODSoundListener::FMODSoundListener()
    {

    }

    FMODSoundListener::~FMODSoundListener()
    {

    }

    void FMODSoundListener::update()
    {
        FMOD::System* system = ((FMODSoundManager*)(getSoundApp()->getSoundManager()))->getFMODSystem();
        if (!system)
            HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "soundsystem null!", "FMODSoundListener::update"); 
        
        FMOD_VECTOR pos;
        pos.x = position.x;
        pos.y = position.y;
        pos.z = 0.0f;
        if(system)
            system->set3DListenerAttributes(0, &pos, NULL, NULL, NULL);

    }

}