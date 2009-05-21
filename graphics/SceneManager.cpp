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
#include "SceneListener.h"
#include "RenderSystem.h"
#include "RenderWindow.h"


namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(SceneManager, Object, 0)
    {
    }

	SceneManager::SceneManager()
		:sceneListener(NULL)
	{

	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::beginScene()
	{
		if (sceneListener){
			sceneListener->beginScene();
		}
	}

	void SceneManager::renderScene()
	{
		Sprite::List::iterator it = spriteList.begin();
		for (;it != spriteList.end(); ++it){
			(*it)->render();
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
		Sprite::List::iterator it = std::find(spriteList.begin(), spriteList.end(), sprite);
		if (spriteList.end() != it)
			spriteList.erase(it);

	}

    void SceneManager::removeAllSprite()
    {
        spriteList.clear();
    }

	void SceneManager::setSceneListener(SceneListenerBase* listener)
	{
		sceneListener = listener;
	}
}
