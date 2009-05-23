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
    class GRAPHICS_API SceneListener : public ReferenceCounted
	{
        HARE_DECLARE_PTR(SceneListener)
	public:
		SceneListener();
		virtual ~SceneListener();

        virtual void beginScene() {};
        virtual void renderScene() {};
        virtual void endScene() {};

        virtual void _doRelease()
        {
            delete this;
        }
	};
}

#endif
