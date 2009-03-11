#include "PCH.h"
#include "HareApp.h"
#include "DevILImageCodec.h"
#include "SceneManager.h"


namespace hare_graphics
{		
	HareApp hareApp;

	typedef void (*DLL_START_PLUGIN)(void);

	HARE_IMPLEMENT_SINGLETON(HareApp)

	HareApp::HareApp()
		:graphicsSystemManager(NULL)
		,plugin(NULL)
	{
		DevILImageCodec::startUp();
	}

	HareApp::~HareApp()
	{
        DevILImageCodec::shutDown();
	}

	void HareApp::setGraphicsSystem(SystemManager* systemManager)
	{
		graphicsSystemManager = systemManager;
	}

	void HareApp::loadPlugin(const String& moduleName)
	{
        plugin = new DynamicLibrary(moduleName);
	}

	void HareApp::freePlugin()
	{
        SAFE_DELETE(plugin);
	}

	RenderWindow* HareApp::createRenderWindow(const WindowParams& params)
	{
		return graphicsSystemManager->createRenderWindow(params);
	}

	SceneManager* HareApp::createSceneManager()
	{
		return graphicsSystemManager->createSceneManager();
	}

	Texture* HareApp::createTexture()
	{
		return graphicsSystemManager->createTexture();
	}

	void HareApp::hareRun()
	{
		graphicsSystemManager->hareRun();
	}

	void HareApp::hareRunFrame()
	{
		graphicsSystemManager->hareRunFrame();
	}

	HareApp* getHareApp()
	{
		return HareApp::getSingletonPtr();
	}


}