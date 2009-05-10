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

namespace hare
{

	class Sprite;
	class SceneListenerBase;

	class GRAPHICS_API SceneManager
	{
	public:
		SceneManager();
	    ~SceneManager();

		void beginScene();

		void renderScene();

		void endScene();

		void addSprite(Sprite* sprite);

		void removeSprite(Sprite* sprite);

		void setSceneListener(SceneListenerBase* listener);

	protected:
		typedef std::list<Sprite*> SpriteList;
		SpriteList spriteList;

		SceneListenerBase* sceneListener;

	};
}

#endif
