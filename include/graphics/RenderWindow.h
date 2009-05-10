//***************************************************************
//  File:    RenderWindow.h
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _RENDERWINDOW_H_
#define _RENDERWINDOW_H_


#include "GraphicsPrerequisites.h"
#include "RenderTarget.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
typedef HWND WindowHandle;
#elif HARE_PLATFORM == HARE_PLATFORM_LINUX
typedef struct _WindowHandle{
	void*   dpy;
	void*   vi;
	int     win;
	_WindowHandle(int x)
		:dpy(NULL)
		,vi(NULL)
		,win(0)
	{
	}
} WindowHandle;
#else
typedef uint32 WindowHandle;
#endif

namespace hare
{
	struct GRAPHICS_API WindowParams
    {
		WindowHandle hwnd;
		bool bFullScreen;
		uint32 width;
		uint32 height;
		bool bZbuffer;
		String title;
		WindowParams()
			:hwnd(0)
			,width(800)
			,height(600)
		{
			title = "hare";
		}
	};

	class SceneManager;

	class GRAPHICS_API RenderWindow : public RenderTarget
	{
	public:
		RenderWindow();

		virtual ~RenderWindow();

		uint32 getWidth()
		{
			return windowParams.width;
		}
		uint32 getHeight()
		{
			return windowParams.height;
		}

		virtual void create(const WindowParams& params) = 0;

		virtual void resize(uint32 w, uint32 h) = 0;

		virtual void swapBuffer() = 0;

		virtual void destoryWindow() = 0;

		void setSceneManager(SceneManager* scene)
		{
			sceneManager = scene;
		}

		SceneManager* getSceneManager()
		{
			return sceneManager;
		}

		bool getIsMainWnd()
		{
			return isMainWnd;
		}

		const WindowParams& getWindowParams()
		{
			return windowParams;
		}

		void moveCamera(float dx, float dy);

        void moveCameraTo(float x, float y);

	protected:
		bool isMainWnd;
        bool isExternal;

		WindowParams windowParams;

		SceneManager* sceneManager;

		PointF cameraPos;
	};
}

#endif
