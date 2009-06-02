//***************************************************************
//  File:    PluginManager.cpp
//  Data:    05/30/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#include "PCH.h"
#include "PluginManager.h"
#include "DynamicLibrary.h"

namespace hare
{
    DynamicLibrary* PluginManager::loadPlugin(const String& fileName)
    {
        PluginLibraryMap::iterator it = pluginMap.find(fileName);
        if (it != pluginMap.end())
        {
            it->second->addRef();
            return it->second;
        }
        else
        {
            DynamicLibrary* plugin = new DynamicLibrary(fileName);
            if (plugin->isLoaded())
            {
                plugin->addRef();
                pluginMap[fileName] = plugin;
            }
            else
            {
                delete plugin;
                plugin = NULL;
            }
            return plugin;
        }
    }

    void PluginManager::freeAllPlugins()
    {
        PluginLibraryMap::iterator it = pluginMap.begin();
        for (; it != pluginMap.end(); ++it)
        {
            it->second->decRef();
        }
        pluginMap.clear();
    }

}
