//***************************************************************
//  File:    Sound3DBuffer.cpp
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
#include "PCH.h"
#include "SoundBuffer.h"
#include "SoundApp.h"
#include "SoundManager.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(SoundBuffer, Object, 0)
    {

    }

    SoundBuffer::SoundBuffer()
    {

    }


	SoundBuffer::~SoundBuffer()
	{

	}

    void SoundBuffer::setSoundParam(SoundParam* param)
    {
        soundParam = param;
    }

}
