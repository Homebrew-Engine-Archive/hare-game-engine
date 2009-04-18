#ifndef SYSTEMMANAGER
#define SYSTEMMANAGER


#include "GraphicsPrerequisites.h"
#include "RenderWindow.h"


namespace hare
{

	class RenderWindow;
	class SceneManager;
	class RenderSystem;
	class Texture;
	class Canvas;


	class GRAPHICS_API SystemManager : public Singleton<SystemManager>
	{
		HARE_DECLARE_SINGLETON(SystemManager)
	public:
		SystemManager();

		virtual ~SystemManager();

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

		bool bResume;
		bool bEnd;
	};
}

#endif
