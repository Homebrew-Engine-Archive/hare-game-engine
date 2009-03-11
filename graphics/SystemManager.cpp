#include "PCH.h"
#include "SystemManager.h"
#include "SceneManager.h"
#include "RenderSystem.h"


namespace hare_graphics
{
	HARE_IMPLEMENT_SINGLETON(SystemManager)

	SystemManager::SystemManager()
		:pPrimaryWindow(NULL)
	{

	}

	SystemManager::~SystemManager()
	{
		SceneManagerList::iterator it = sceneManagerList.begin();
		for (;it != sceneManagerList.end(); ++it){
			delete *it;
		}
		sceneManagerList.clear();
	}

	SceneManager* SystemManager::createSceneManager()
	{
		SceneManager* scene = new SceneManager;
		sceneManagerList.push_back(scene);
		return scene;
	}

	void SystemManager::hareRunFrame()
	{
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
			renderSystem->setRenderTarget( *it );
			if ((*it)->getSceneManager())
				(*it)->getSceneManager()->renderScene();
		}

		if (pPrimaryWindow){
			renderSystem->setRenderTarget(pPrimaryWindow);
			if (pPrimaryWindow->getSceneManager())
				pPrimaryWindow->getSceneManager()->renderScene();
		}

		renderSystem->setRenderTarget(NULL);

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