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
        HARE_OBJ_LIST(spriteList, Sprite)
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

	int  SceneManager::addSprite(Sprite* sprite)
	{
        if (std::find(spriteList.begin(), spriteList.end(), sprite) == spriteList.end()){
			spriteList.push_back(sprite);
            return spriteList.size() - 1;
        }
        return -1;       
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

    Sprite* SceneManager::getSpriteByName(const String& name)
    {
        Sprite::List::iterator it = spriteList.begin();
        for (; it != spriteList.end(); ++it){
            if ((*it)->getSpriteName() == name){
                return *it;
            }
        }
        return NULL;
    }

    Sprite* SceneManager::getSpriteByID(int id)
    {
        if (id < 0 || id >= (int)spriteList.size())
            return NULL;

        Sprite::List::iterator it = spriteList.begin();
        for (int i = 0;it != spriteList.end(); ++it, ++i){
            if (i == id)
                return *it;
        }

        return NULL;
    }

    int SceneManager::getSpriteCount()
    {
        return spriteList.size();
    }

}
