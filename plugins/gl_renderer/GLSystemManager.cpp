#include "PCH.h"
#include "GLSystemManager.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#include "Win32/GLRenderWindow.h"
#elif HARE_PLATFORM == HARE_PLATFORM_LINUX
#include "Linux/GLRenderWindow.h"
#elif HARE_PLATFORM == HARE_PLATFORM_PSP
#include "PSP/GLRenderWindow.h"
#endif

#include "GLRenderSystem.h"
#include "GLTexture.h"
#include "GLVertexBufferManager.h"


HARE_IMPLEMENT_DYNAMIC_CLASS(GLSystemManager, SystemManager, 0)
{
}

GLVertexBufferManager* vertexBufferManager;

GLSystemManager::GLSystemManager()
{
	renderSystem = new GLRenderSystem;
	renderSystem->initalize();
	vertexBufferManager = NULL;
}

GLSystemManager::~GLSystemManager()
{
	if (pPrimaryWindow){
		delete pPrimaryWindow;
		pPrimaryWindow = NULL;
	}

	if (vertexBufferManager){
	    delete vertexBufferManager;
        vertexBufferManager = NULL;
	}

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
        vertexBufferManager = new GLVertexBufferManager;
	}else{
		secondaryWindowList.push_back(window);
	}

	return window;
}

void GLSystemManager::destoryRenderWindow(RenderWindow* renderWindow)
{
	//不能在这销毁主窗口
	//assert(renderWindow != pPrimaryWindow && pPrimaryWindow != NULL);

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

int  GLSystemManager::hareRunFrame()
{
	return SystemManager::hareRunFrame();
}

void GLSystemManager::hareRun()
{
#if HARE_PLATFORM == HARE_PLATFORM_WIN32
	MSG Message;

	for (;;){
		if (PeekMessage(&Message, 0, 0, 0, PM_NOREMOVE)){
			if (!GetMessage(&Message, NULL, 0, 0)){
				break;
			}
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		} else {
			int ret = hareRunFrame();

			if (ret == 0)
				break;
		}
	}
#elif HARE_PLATFORM == HARE_PLATFORM_PSP

	do{
		int ret = hareRunFrame();

		if (ret == 0)
			break;
	}while(1);

#elif HARE_PLATFORM == HARE_PLATFORM_LINUX

    while(1){
        Display* dpy = ((GLRenderWindow*)pPrimaryWindow)->getDisplay();
        if (XPending(dpy) > 0)
		{
            XEvent event;
            XNextEvent(dpy, &event);
			GLXProc(event, (GLRenderWindow*)pPrimaryWindow);
            //break;
        }

		SecondaryWindowList::iterator it = secondaryWindowList.begin();
		for (;it != secondaryWindowList.end(); ++it){
            GLRenderWindow* glwindow = (GLRenderWindow*)(*it);
			dpy = glwindow->getDisplay();
			if (XPending(dpy) > 0)
			{
				XEvent event;
				XNextEvent(dpy, &event);
				GLXProc(event, glwindow);
				break;
			}
		}

        int ret = hareRunFrame();

		if (ret == 0)
			break;
    }
#endif
}


