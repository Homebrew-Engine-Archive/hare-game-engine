#include "PCH.h"
#include "SystemManager.h"
#include "SceneManager.h"
#include "RenderSystem.h"
#include "Canvas.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_SINGLETON(SystemManager)

	SystemManager::SystemManager()
		:pPrimaryWindow(NULL)
	{
		canvas = new Canvas;
	}

	SystemManager::~SystemManager()
	{
		SceneManagerList::iterator it = sceneManagerList.begin();
		for (;it != sceneManagerList.end(); ++it){
			delete *it;
		}
		sceneManagerList.clear();
		delete canvas;
	}

	SceneManager* SystemManager::createSceneManager()
	{
		SceneManager* scene = new SceneManager;
		sceneManagerList.push_back(scene);
		return scene;
	}

	void SystemManager::hareRunFrame()
	{
        getTimer().update();

		//äÖÈ¾Ç°
		SecondaryWindowList::iterator it = secondaryWindowList.begin();
		for (;it != secondaryWindowList.end(); ++it){
			if ((*it)->getSceneManager())
				(*it)->getSceneManager()->beginScene();
		}

		if (pPrimaryWindow){
			if (pPrimaryWindow->getSceneManager())
				pPrimaryWindow->getSceneManager()->beginScene();
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

		//renderSystem->setRenderTarget(NULL);

		//äÖÈ¾ºó
		it = secondaryWindowList.begin();
		for (;it != secondaryWindowList.end(); ++it){
			if ((*it)->getSceneManager())
				(*it)->getSceneManager()->endScene();
		}

		if (pPrimaryWindow){
			if (pPrimaryWindow->getSceneManager())
				pPrimaryWindow->getSceneManager()->endScene();
		}
	}
}