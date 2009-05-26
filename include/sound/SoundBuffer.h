//***************************************************************
//  File:    SoundBuffer.h
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
#ifndef	_SOUNDBUFFER_H_
#define _SOUNDBUFFER_H_

#include "SoundPrerequisites.h"
#include "SoundParam.h"

namespace hare
{
    class SOUND_API SoundBuffer : public Object
	{
        HARE_DECLARE_ABSTRACT_CLASS(SoundBuffer)
	public:
        SoundBuffer();
		virtual ~SoundBuffer();

        void setSoundParam(SoundParam* param);

        virtual void load() = 0;

        virtual void unload() = 0;

        virtual void update() = 0;

    protected:
        SoundParam::Ptr soundParam;
	};
}

#endif
