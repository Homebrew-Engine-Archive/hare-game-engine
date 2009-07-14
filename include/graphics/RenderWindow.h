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
#include "SceneManager.h"

namespace hare
{
    class GRAPHICS_API WindowParams
    {
    public:
        uint32 width;
        uint32 height;
        String title;
        bool fullScreen;
        bool hasZbuffer;

        void setCustomData(const String& key, const String& val);
        String getCustomData(const String& key);
        bool hasCustomData(const String& key);

    protected:
        typedef std::map<String, String> ParamList;

        ParamList customData;

    public:
        WindowParams() : width(800), height(600), title("hare game engine"),
            fullScreen(false), hasZbuffer(false) {}
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

		virtual bool getCustomData(const String& key, void* data)
		{
            return false;
        }

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

        const PointF& getCameraPos();

	protected:
		bool isMainWnd;
        bool isExternal;

		WindowParams windowParams;

        SceneManager::Ptr sceneManager;

		PointF cameraPos;
	};
}

#endif
