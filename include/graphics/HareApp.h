#ifndef HAREAPP
#define HAREAPP

#include "GraphicsPrerequisites.h"
#include "RenderWindow.h"
#include "SystemManager.h"

typedef void (*pluginCallback)();

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

                void setPluginCallback(pluginCallback callback)
                {
                    this->callback = callback;
                }
                void createPlugin()
                {
                    if (callback)callback();
                }

	private:
		DynamicLibrary* plugin;
		SystemManager* graphicsSystemManager;
		TextManager* textManager;

                pluginCallback callback;
	};

	GRAPHICS_API HareApp* getHareApp();

}

#endif
