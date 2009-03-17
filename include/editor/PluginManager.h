//***************************************************************
//  File:    PluginManager.h
//  Data:    11/04/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _PLUGINMANAGER_H_
#define _PLUGINMANAGER_H_

#include "EditorPrerequisites.h"
#include "TManager.h"
#include "EditorPlugin.h"
#include <vector>

namespace hare_editor
{
    #define PLUGIN_SDK_VERSION_MAJOR    0
    #define PLUGIN_SDK_VERSION_MINOR    0
    #define PLUGIN_SDK_VERSION_RELEASE  1

    typedef EditorPlugin*   (*FNcreatePlugin)(void);
    typedef void            (*FNfreePlugin)(EditorPlugin*);
    typedef void            (*FNgetVersion)(int*, int*, int*);

    struct PluginInfo
    {
        wxString name;
        wxString title;
        wxString description;
    };

    struct PluginElement
    {
        PluginInfo info;
        wxString fileName;
        EditorPlugin* plugin;
        FNfreePlugin freeProc;
    };
    WX_DEFINE_ARRAY(PluginElement*, PluginElementsArray);
    WX_DEFINE_ARRAY(EditorPlugin*, PluginsArray);

    class EDITOR_API PluginManager : public TManager<PluginManager>
    {
        friend class TManager<PluginManager>;

    public:
        int scanForPlugins(const wxString& path);
        void loadAllPlugins();

        EditorPlugin* findPluginByName(const wxString& pluginName);
        EditorPlugin* findPluginByFileName(const wxString& pluginFileName);

        const PluginInfo* getPluginInfo(EditorPlugin* plugin);

        PluginsArray findPlugins(EditorPluginType type);

        void registerPlugin(const PluginInfo& info, FNcreatePlugin fncreatePlugin, 
            FNfreePlugin fnfreePlugin, FNgetVersion fngetVersion);

    private:
        void unloadAllPlugins();
        bool loadPlugin(const wxString& fileName);
        void unloadPlugin(PluginElement* elem);

        bool attachPlugin(EditorPlugin* plugin);
        bool detachPlugin(EditorPlugin* plugin);

        struct PluginRegistration
        {
            PluginRegistration() : fncreatePlugin(0), fnfreePlugin(0), fngetVersion(0)
            {
            }
            PluginRegistration(const PluginRegistration& rhs)
                : fncreatePlugin(rhs.fncreatePlugin), fnfreePlugin(rhs.fnfreePlugin),
                fngetVersion(rhs.fngetVersion), info(rhs.info)
            {
            }
            FNcreatePlugin fncreatePlugin;
            FNfreePlugin fnfreePlugin;
            FNgetVersion fngetVersion;
            PluginInfo info;
        };

        std::vector<PluginRegistration> registeredPlugins;

        PluginElementsArray plugins;

    private:
         PluginManager();
        ~PluginManager();
    };

    template<typename T> class PluginRegistrant
    {
    public:
        PluginRegistrant(const PluginInfo& info)
        {
            Manager::getInstancePtr()->getPluginManager()->registerPlugin(
                info,           // plugin's info
                &createPlugin,  // creation
                &freePlugin,    // destruction
                &getVersion);   // version
        }

        static EditorPlugin* createPlugin()
        {
            return new T;
        }

        static void freePlugin(EditorPlugin* plugin)
        {
            delete plugin;
        }

        static void getVersion(int* major, int* minor, int* release)
        {
            if (major) *major = PLUGIN_SDK_VERSION_MAJOR;
            if (minor) *minor = PLUGIN_SDK_VERSION_MINOR;
            if (release) *release = PLUGIN_SDK_VERSION_RELEASE;
        }
    };

}

#endif
