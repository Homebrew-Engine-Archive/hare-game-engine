//***************************************************************
//  File:    SystemManager.cpp
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
#include "SystemManager.h"
#include "SceneManager.h"
#include "RenderSystem.h"
#include "Canvas.h"
#include "TextManager.h"

namespace hare
{
	HARE_IMPLEMENT_SINGLETON(SystemManager)

    HARE_IMPLEMENT_ABSTRACT_CLASS(SystemManager, Object, 0)
    {
    }

	SystemManager::SystemManager()
		:pPrimaryWindow(NULL)
		,bResume(true)
	    ,bEnd(false)
	{

	}

	SystemManager::~SystemManager()
	{

	}

	void SystemManager::startUp()
	{
		canvas      = new Canvas;
		textManager = new TextManager();
	}

	void SystemManager::shutDown()
	{
		SceneManagerList::iterator it = sceneManagerList.begin();
		for (;it != sceneManagerList.end(); ++it){
			delete *it;
		}
		sceneManagerList.clear();

        delete textManager;
		delete canvas;
	}

	SceneManager* SystemManager::createSceneManager()
	{
		SceneManager* scene = new SceneManager;
		sceneManagerList.push_back(scene);
		return scene;
	}

	int  SystemManager::hareRunFrame()
	{
        getTimer().update();

		if (bEnd)
			return 0;

		if (!bResume)
			return 2;

		//äÖÈ¾Ç°
		SecondaryWindowList::iterator it = secondaryWindowList.begin();
		for (;it != secondaryWindowList.end(); ++it){
			if ((*it)->getSceneManager()){
                renderSystem->setCurRenderWindow(*it);
				(*it)->getSceneManager()->beginScene();
            }
		}

		if (pPrimaryWindow){
            if (pPrimaryWindow->getSceneManager()) {
                renderSystem->setCurRenderWindow(pPrimaryWindow);
				pPrimaryWindow->getSceneManager()->beginScene();
            }
		}

		//äÖÈ¾
		it = secondaryWindowList.begin();
		for (;it != secondaryWindowList.end(); ++it){
			RenderWindow* renderWindow = *it;

			renderWindow->active();

			if ((*it)->getSceneManager()){
				(*it)->getSceneManager()->renderScene();
			}

			renderWindow->inactive();

			renderWindow->swapBuffer();

		}

		if (pPrimaryWindow){
			pPrimaryWindow->active();

			if (pPrimaryWindow->getSceneManager()){
				pPrimaryWindow->getSceneManager()->renderScene();
			}

			pPrimaryWindow->inactive();

			pPrimaryWindow->swapBuffer();
		}

		//äÖÈ¾ºó
		it = secondaryWindowList.begin();
		for (;it != secondaryWindowList.end(); ++it){
            if ((*it)->getSceneManager()) {
                renderSystem->setCurRenderWindow(*it);
                (*it)->getSceneManager()->endScene();
            }
		}

		if (pPrimaryWindow){
            if (pPrimaryWindow->getSceneManager()) {
				renderSystem->setCurRenderWindow(pPrimaryWindow);
                pPrimaryWindow->getSceneManager()->endScene();
            }
		}

        renderSystem->setCurRenderWindow(NULL);
		return 1;
	}

	RenderWindow* SystemManager::getPrimaryWindow()
	{
		return pPrimaryWindow;
	}

	void SystemManager::pause()
	{
        bResume = true;
	}

	void SystemManager::resume()
	{
		bResume = false;
	}

	void SystemManager::end()
	{
		bEnd = true;
	}
}
