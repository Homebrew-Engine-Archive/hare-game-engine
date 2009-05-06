#ifndef _SYSTEMMANAGER_H_
#define _SYSTEMMANAGER_H_


#include "GraphicsPrerequisites.h"
#include "RenderWindow.h"


namespace hare
{

	class RenderWindow;
	class SceneManager;
	class RenderSystem;
	class Texture;
	class Canvas;
	class TextManager;


	class GRAPHICS_API SystemManager : public Object, public Singleton<SystemManager>
	{
        HARE_DECLARE_ABSTRACT_CLASS(SystemManager)

		HARE_DECLARE_SINGLETON(SystemManager)

    public:
		SystemManager();

		virtual ~SystemManager();

		//here: init some system which are not be inherited
		void startUp();

		void shutDown();

		virtual RenderWindow* createRenderWindow(const WindowParams& params) = 0;

		virtual void destoryRenderWindow(RenderWindow* renderWindow) = 0;

		SceneManager* createSceneManager();

		virtual Texture* createTexture() = 0;

		virtual int hareRunFrame();

		virtual void hareRun() = 0;

		void pause();

		void resume();

		void end();

		RenderWindow* getPrimaryWindow();

	protected:

		typedef std::list<SceneManager*> SceneManagerList;
		SceneManagerList sceneManagerList;

		RenderWindow* pPrimaryWindow;

		typedef std::vector<RenderWindow*> SecondaryWindowList;
		SecondaryWindowList secondaryWindowList;

		RenderSystem* renderSystem;
		Canvas*       canvas;
		TextManager*  textManager;

		bool bResume;
		bool bEnd;
	};
}

#endif
