#include "PCH.h"
#include "HareApp.h"
#include "DevILImageCodec.h"
#include "SceneManager.h"
#include "TextManager.h"
#include "Canvas.h"

namespace hare_graphics
{
	HareApp hareApp;

	HARE_IMPLEMENT_SINGLETON(HareApp)

	HareApp::HareApp()
		: plugin(NULL)
		, graphicsSystemManager(NULL)
	{

	}

	HareApp::~HareApp()
	{

	}

	void HareApp::startUp()
	{
		DevILImageCodec::startUp();
		textManager = new TextManager();
	}

	void HareApp::shutDown()
	{
		delete textManager;
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
