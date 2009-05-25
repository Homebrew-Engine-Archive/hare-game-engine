//***************************************************************
//  File:    SoundApp.cpp
//  Date:    5/24/2009
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
#include "SoundApp.h"

namespace hare
{
	HARE_IMPLEMENT_SINGLETON(SoundApp)

    SoundApp::SoundApp()
    {

    }

    SoundApp::~SoundApp()
    {

    }

    void SoundApp::startUp()
    {
        ClassInfoList subSystems;
        SoundManager::CLASS_INFO.findSubs(subSystems);
        if (subSystems.size() > 0){
            soundManager = (SoundManager*)subSystems[0]->createObject();
            soundManager->initialize();
        }else{
            Log::getSingleton().logError("SoundApp::startUp no soundsystem loaded");
        }
    }

    void SoundApp::shutDown()
    {
        if (soundManager)
            soundManager->release();
        soundManager = NULL;
    }

    DynamicLibrary* SoundApp::loadPlugin(const String& moduleName)
    {
        PluginMap::iterator it = pluginMap.find(moduleName);
        if (it != pluginMap.end()){
            it->second->addRef();
            return it->second;
        }else{
            DynamicLibrary* plugin = new DynamicLibrary(moduleName);
            if (plugin->isLoaded()){
                plugin->addRef();
                pluginMap[moduleName] = plugin;
            }else{
                delete plugin;
                plugin = NULL;
            }
            return plugin;
        }
    }

    void SoundApp::freeAllPlugins()
    {
        PluginMap::iterator it = pluginMap.begin();
        for (; it != pluginMap.end(); ++it){
            it->second->decRef();
        }
        pluginMap.clear();
    }

    SoundManager* SoundApp::getSoundManager()
    {
        return soundManager;
    }

    SoundApp* getSoundApp()
    {
        static SoundApp soundApp;
        return &soundApp;
    }

    void sound_init()
    {
        getSoundApp()->startUp();
    }

    void sound_quit()
    {
        getSoundApp()->shutDown();
    }
}