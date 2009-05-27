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
#include "SoundParam.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(SoundParam, Object, 0)
    {
        HARE_META(fileName, String)
        HARE_META(innerCone, float)
        HARE_META(outerCone, float)
        HARE_META(minGain, float)
        HARE_META(maxGain, float)
        HARE_META(refDistance, float)
        HARE_META(maxDistance, float)
        HARE_META(pitch, float)
        HARE_META(gain, float)
        HARE_META(outerGain, float)
        HARE_META(rolloff, float)
        HARE_META(bFromStream, bool)
    }


    SoundParam::SoundParam()
        :innerCone(360.0f)
        ,outerCone(360.0f)
        ,minGain(0.0f)
        ,maxGain(1.0f)
        ,refDistance(100.0f)
        ,maxDistance(1000.0f)
        ,pitch(1.0f)
        ,gain(1.0f)
        ,outerGain(0.0f)
        ,rolloff(1.0f)
        ,bFromStream(false)
    {

    }

    SoundParam::~SoundParam()
    {

    }

    void SoundParam::postEdited(Attribute *attr)
    {

    }

    void SoundParam::postLoaded()
    {

    }
}