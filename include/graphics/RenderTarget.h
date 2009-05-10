//***************************************************************
//  File:    RenderTarget.h
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
#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "GraphicsPrerequisites.h"

namespace hare
{
	class GRAPHICS_API RenderTarget
	{
	public:
		RenderTarget();
		virtual ~RenderTarget();

		virtual void active() = 0;
		virtual void inactive() = 0;
	};
}

#endif
