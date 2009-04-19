#include "PCH.h"
#include "HareApp.h"
#include "DevILImageCodec.h"
#include "SceneManager.h"
#include "TextManager.h"
#include "Canvas.h"

namespace hare
{
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

	int  HareApp::hareRunFrame()
	{
		return graphicsSystemManager->hareRunFrame();
	}

	void HareApp::pause()
	{
		graphicsSystemManager->pause();
	}

	void HareApp::resume()
	{
		graphicsSystemManager->resume();
	}

	void HareApp::end()
	{
		graphicsSystemManager->end();
	}

	HareApp* getHareApp()
	{
		static HareApp hareApp;
		return HareApp::getSingletonPtr();
	}
}


#include <stdlib.h>

void __pspgl_log (const char *fmt, ...)
{
	va_list ap;

#if 0
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#else
	char buf [1024];
	int len;
	FILE* log_fd;

	va_start(ap, fmt);
	len = _vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	log_fd = fopen("ms0:/log.txt", "a");
	fwrite(buf, 1, len, log_fd);
	fclose(log_fd);
#endif
}
