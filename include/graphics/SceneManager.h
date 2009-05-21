//***************************************************************
//  File:    SceneManager.h
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
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_


#include "GraphicsPrerequisites.h"
#include "Sprite.h"

namespace hare
{
	class SceneListenerBase;

    class GRAPHICS_API SceneManager : public Object
	{
        HARE_DECLARE_DYNAMIC_CLASS(SceneManager)
	public:
		SceneManager();
	    ~SceneManager();

		void beginScene();

		void renderScene();

		void endScene();

		void addSprite(Sprite* sprite);

		void removeSprite(Sprite* sprite);

        void removeAllSprite();

		void setSceneListener(SceneListenerBase* listener);

	protected:
        Sprite::List spriteList;

		SceneListenerBase* sceneListener;

	};
}

#endif
