#include "PCH.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SceneListener.h"


namespace hare_graphics
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
		if (sceneListener){
			sceneListener->renderScene();
		}
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