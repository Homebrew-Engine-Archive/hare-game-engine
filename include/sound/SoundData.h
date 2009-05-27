//***************************************************************
//  File:    SoundData.h
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
#ifndef _SOUNDDATA_H_
#define _SOUNDDATA_H_

#include "SoundPrerequisites.h"
#include "SoundParam.h"
#include "SoundBuffer.h"

namespace hare
{


    class SOUND_API SoundData : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(SoundData)
    public:
        SoundData();
        virtual ~SoundData();

        void setSoundParam(SoundParam* param);

        SoundBuffer* getSoundBuffer();

        virtual void postEdited(Attribute *attr);

        virtual void postLoaded();

    protected:
        SoundParam::Ptr soundParam;

        SoundBuffer::Ptr soundBuffer;

    };
}

#endif