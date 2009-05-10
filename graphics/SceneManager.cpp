//***************************************************************
//  File:    SceneManager.cpp
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
#include "PCH.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SceneListener.h"
#include "RenderSystem.h"
#include "RenderWindow.h"


namespace hare
{
	SceneManager::SceneManager()
		:sceneListener(NULL)
	{

	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::beginScene()
	{
		SpriteList::iterator it = spriteList.begin();
		for (;it != spriteList.end(); ++it){
			(*it)->beginScene();
		}
		if (sceneListener){
			sceneListener->beginScene();
		}
	}

	void SceneManager::renderScene()
	{
		SpriteList::iterator it = spriteList.begin();
		for (;it != spriteList.end(); ++it){
			(*it)->renderScene();
		}
        RenderSystem::getSingletonPtr()->render();

        RenderSystem::getSingletonPtr()->prepareCanvasRender();
		if (sceneListener){
			sceneListener->renderScene();
		}
        RenderSystem::getSingletonPtr()->render();
	}

	void SceneManager::endScene()
	{
		SpriteList::iterator it = spriteList.begin();
		for (;it != spriteList.end(); ++it){
			(*it)->endScene();
		}
		if (sceneListener){
			sceneListener->endScene();
		}
	}

	void SceneManager::addSprite(Sprite* sprite)
	{
		if (std::find(spriteList.begin(), spriteList.end(), sprite) == spriteList.end())
			spriteList.push_back(sprite);
	}

	void SceneManager::removeSprite(Sprite* sprite)
	{
		SpriteList::iterator it = std::find(spriteList.begin(), spriteList.end(), sprite);
		if (spriteList.end() != it)
			spriteList.erase(it);

	}

	void SceneManager::setSceneListener(SceneListenerBase* listener)
	{
		sceneListener = listener;
	}
}
