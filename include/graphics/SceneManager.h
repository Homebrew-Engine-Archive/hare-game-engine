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
