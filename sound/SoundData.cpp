//***************************************************************
//  File:    SoundData.cpp
//  Date:    5/26/2009
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
#include "SoundData.h"
#include "SoundApp.h"
#include "SoundManager.h"

namespace hare
{

    HARE_IMPLEMENT_DYNAMIC_CLASS(SoundData, Object, 0)
    {
        HARE_OBJ_F(soundParam, SoundParam, propAvoidNull);
    }
    
    SoundData::SoundData()
    {
        soundBuffer = getSoundApp()->getSoundManager()->createSoundBuffer();
    }

    SoundData::~SoundData()
    {

    }

    void SoundData::setSoundParam(SoundParam* param)
    {
        soundParam = param;
    }

    void SoundData::postEdited(Attribute *attr)
    {
        postLoaded();
    }

    void SoundData::postLoaded()
    {   
        soundBuffer->setSoundParam(soundParam);
        soundBuffer->unload();
        soundBuffer->load();
        soundBuffer->update();
    }

    SoundBuffer* SoundData::getSoundBuffer()
    {
        return soundBuffer;
    }
}