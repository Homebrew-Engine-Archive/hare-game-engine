//***************************************************************
//  File:    GraphicsUtil.h
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _GRAPHICSUTIL_H_
#define _GRAPHICSUTIL_H_

#include "GraphicsPrerequisites.h"
#include "HareGraphicsType.h"

namespace hare
{
    /** The class is Graphics Utility
    */
	class GRAPHICS_API GraphicsUtil
	{
	public:
		static uint32 getPixelFormatBytes(HarePixelFormat hpf);
	};

}
#endif
