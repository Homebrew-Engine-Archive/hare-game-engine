//***************************************************************
//  File:    PluginManager.h
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

#include "CorePrerequisites.h"
#include "Singleton.h"

namespace hare
{
    /** Class for plugin management
    */
    class CORE_API PluginManager : public Singleton<PluginManager>
    {
    public:
        /// Load a plugin and return the DynamicLibrary
        DynamicLibrary* loadPlugin(const String& fileName);

        /// Free all loaded plugins by decreasing DynamicLibrary's ref-count
        void freeAllPlugins();

    private:
        typedef std::map<String, DynamicLibrary*> PluginLibraryMap;
        PluginLibraryMap pluginMap;
    };
}