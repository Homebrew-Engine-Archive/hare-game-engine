//***************************************************************
//  File:    SoundApp.h
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
#ifndef _SOUNDAPP_H_
#define _SOUNDAPP_H_

#include "SoundPrerequisites.h"
#include "SoundManager.h"


namespace hare
{
    class SOUND_API SoundApp : public Singleton<SoundApp>
    {
		HARE_DECLARE_SINGLETON(SoundApp)
    public:
        SoundApp();
        ~SoundApp();

        //first loadplugin
        //second startUp
        //init plugin system exp: fmod openAL 
        void startUp();

        void shutDown();

        DynamicLibrary* loadPlugin(const String& moduleName);

        void freeAllPlugins();

        SoundManager* getSoundManager();

    protected:
        typedef std::map<String, DynamicLibrary*> PluginMap;
        PluginMap pluginMap;

        SoundManager::Ptr soundManager;
    };

    SOUND_API SoundApp* getSoundApp();

    SOUND_API void sound_init();

    SOUND_API void sound_quit();
}

#endif
