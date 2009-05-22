//***************************************************************
//  File:    HareApp.cpp
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
	{
	}

	HareApp::~HareApp()
	{
	}

	void HareApp::startUp()
	{
		DevILImageCodec::startUp();

        ClassInfoList subSystems;
        SystemManager::CLASS_INFO.findSubs(subSystems);
        if (subSystems.size() > 0)
        {
            graphicsSystemManager = (SystemManager*)subSystems[0]->createObject();
            graphicsSystemManager->startUp();
        }
        else
        {
            Log::getSingleton().logError("HareApp::startUp no renderer loaded");
        }
	}

	void HareApp::shutDown()
	{
        if (graphicsSystemManager)
		    graphicsSystemManager->shutDown();
        graphicsSystemManager = NULL;
        DevILImageCodec::shutDown();
	}

	/*void HareApp::setGraphicsSystem(SystemManager* systemManager)
	{
		graphicsSystemManager = systemManager;
	}*/

	DynamicLibrary* HareApp::loadPlugin(const String& moduleName)
	{
        PluginMap::iterator it = pluginMap.find(moduleName);
        if (it != pluginMap.end())
        {
            it->second->addRef();
            return it->second;
        }
        else
        {
            DynamicLibrary* plugin = new DynamicLibrary(moduleName);
            if (plugin->isLoaded())
            {
                plugin->addRef();
                pluginMap[moduleName] = plugin;
            }
            else
            {
                delete plugin;
                plugin = NULL;
            }
            return plugin;
        }
	}

    void HareApp::freeAllPlugins()
	{
        PluginMap::iterator it = pluginMap.begin();
        for (; it != pluginMap.end(); ++it)
        {
            it->second->decRef();
        }
        pluginMap.clear();
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
	    if (graphicsSystemManager)
            graphicsSystemManager->hareRun();
	}

	int  HareApp::hareRunFrame()
	{
	    if (graphicsSystemManager)
            return graphicsSystemManager->hareRunFrame();
		return 1;
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

	void graphics_init()
	{
        getHareApp()->startUp();
	}

	void graphics_quit()
	{
        getHareApp()->shutDown();
	}
}
