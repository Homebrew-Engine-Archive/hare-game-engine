//***************************************************************
//  File:    SoundParam.h
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
#ifndef _SOUNDPARAM_H_
#define _SOUNDPARAM_H_

#include "SoundPrerequisites.h"


namespace hare
{
    class SOUND_API SoundParam : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(SoundParam)
    public:
        SoundParam();
        virtual ~SoundParam();

        String  fileName;

        float   innerCone;
        float   outerCone;
        float   minGain;
        float   maxGain;
        float   refDistance;
        float   maxDistance;
        float   pitch;
        float   gain;
        float   outerGain;
        float   rolloff;
        bool    bFromStream;

        virtual void postEdited(Attribute *attr);

        virtual void postLoaded();
    };
}


#endif


