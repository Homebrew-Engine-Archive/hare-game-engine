#ifndef HAREAPP
#define HAREAPP

#include "GraphicsPrerequisites.h"
#include "RenderWindow.h"
#include "SystemManager.h"

namespace hare
{
	class SystemManager;
	class SceneManager;
	class Texture;
	class TextManager;


	class GRAPHICS_API HareApp : public Singleton<HareApp>
	{
		HARE_DECLARE_SINGLETON(HareApp)
	public:
		HareApp();

		~HareApp();

		//first loadplugin
		//second startUp
		void startUp();

		void shutDown();

		void setGraphicsSystem(SystemManager* systemManager);

		void loadPlugin(const String& moduleName);

		void freePlugin();

		RenderWindow* createRenderWindow(const WindowParams& params);

		SceneManager* createSceneManager();

		Texture* createTexture();

		void hareRun();

		int  hareRunFrame();

		void pause();

		void resume();

		void end();

	private:
		DynamicLibrary* plugin;
		SystemManager* graphicsSystemManager;
		TextManager* textManager;
	};

	GRAPHICS_API HareApp* getHareApp();

}

#endif
