//***************************************************************
//  File:    HareApp.h
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
#ifndef _HAREAPP_H_
#define _HAREAPP_H_

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

		DynamicLibrary* loadPlugin(const String& moduleName);

		void freeAllPlugins();

		RenderWindow* createRenderWindow(const WindowParams& params);

		SceneManager* createSceneManager();

		Texture* createTexture();

		void hareRun();

		int  hareRunFrame();

		void pause();

		void resume();

		void end();

	private:
        typedef std::map<String, DynamicLibrary*> PluginMap;
        PluginMap pluginMap;

        SystemManager::Ptr graphicsSystemManager;
	};

	GRAPHICS_API HareApp* getHareApp();

    GRAPHICS_API void graphics_init();

    GRAPHICS_API void graphics_quit();
}

#endif
