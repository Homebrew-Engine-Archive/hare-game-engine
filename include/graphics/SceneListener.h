//***************************************************************
//  File:    SceneListener.h
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
#ifndef _SCENELISTENER_H_
#define _SCENELISTENER_H_

#include "GraphicsPrerequisites.h"

namespace hare
{
	class GRAPHICS_API SceneListenerBase
	{
	public:
		SceneListenerBase();
		virtual ~SceneListenerBase();

		virtual void beginScene() = 0;
		virtual void renderScene() = 0;
		virtual void endScene() = 0;

	};
}

#endif
