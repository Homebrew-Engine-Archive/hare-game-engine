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
		//init plugin system exp: d3d gl SceneManger Particle
		//NB: d3d or gl must be initalized first
		void startUp();

		//NB: d3d or gl must be released in the end
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

	};

	GRAPHICS_API HareApp* getHareApp();

    GRAPHICS_API void graphics_init();

    GRAPHICS_API void graphics_quit();
}

#endif
