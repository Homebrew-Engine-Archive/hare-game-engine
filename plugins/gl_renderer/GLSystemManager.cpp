#include "PCH.h"
#include "GLSystemManager.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#include "Win32/GLRenderWindow.h"
#endif

#include "GLRenderSystem.h"
#include "GLTexture.h"

GLSystemManager::GLSystemManager()
{
	renderSystem = new GLRenderSystem;
	renderSystem->initalize();
}

GLSystemManager::~GLSystemManager()
{
	delete pPrimaryWindow;
	pPrimaryWindow = NULL;
	SecondaryWindowList::iterator it = secondaryWindowList.begin();
	for (;it != secondaryWindowList.end(); ++it){
		delete *it;
	}
	secondaryWindowList.clear();

	renderSystem->release();
	delete renderSystem;
}

RenderWindow* GLSystemManager::createRenderWindow(const WindowParams& params)
{
	GLRenderWindow* window = new GLRenderWindow(pPrimaryWindow == NULL);

	window->create(params);

	if (window->getIsMainWnd()){
		pPrimaryWindow = window;
	}else{
		secondaryWindowList.push_back(window);
	}

	return window;
}

void GLSystemManager::destoryRenderWindow(RenderWindow* renderWindow)
{
	//不能在这销毁主窗口
	assert(renderWindow != pPrimaryWindow && pPrimaryWindow != NULL);

	SecondaryWindowList::iterator it = secondaryWindowList.begin();
	for (;it != secondaryWindowList.end(); ++it){
		if (renderWindow == (*it)){
			delete *it;
			secondaryWindowList.erase(it);
			break;
		}
	}
}

Texture* GLSystemManager::createTexture()
{
	return RenderSystem::getSingletonPtr()->createTexture();
}

void GLSystemManager::hareRunFrame()
{
	SystemManager::hareRunFrame();
}

void GLSystemManager::hareRun()
{
	MSG Message;

	for (;;){	
		if (PeekMessage(&Message, 0, 0, 0, PM_NOREMOVE)){
			if (!GetMessage(&Message, NULL, 0, 0)){
				break;
			}
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		} else {
			hareRunFrame();
		}
	}
}


