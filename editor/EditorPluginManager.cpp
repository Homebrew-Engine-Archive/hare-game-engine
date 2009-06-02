//***************************************************************
//  File:    EditorPluginManager.cpp
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
#include "PCH.h"
#include "EditorPluginManager.h"
#include "EditorManager.h"
#include "EditorPlugin.h"
#include <wx/dynload.h>
#include <wx/dir.h>
#include <wx/filename.h>

namespace hare
{
    template<> EditorPluginManager* TManager<EditorPluginManager>::instance = 0;
    template<> bool TManager<EditorPluginManager>::autoCreate = true;

    EditorPluginManager::EditorPluginManager()
    {
    }

    EditorPluginManager::~EditorPluginManager()
    {
        unloadAllPlugins();
    }

    bool EditorPluginManager::attachPlugin(EditorPlugin* plugin)
    {
        if (!plugin)
            return false;
        if (plugin->isAttached())
            return true;

        plugin->attach();
        return true;
    }

    bool EditorPluginManager::detachPlugin(EditorPlugin* plugin)
    {
        if (!plugin)
            return false;
        if (!plugin->isAttached())
            return true;

        Manager::getInstancePtr()->removeAllEventSinksFor(plugin);
        plugin->detach(Manager::isAppShuttingDown());
        return true;
    }

    void EditorPluginManager::unloadAllPlugins()
    {
        while (plugins.GetCount())
        {
            unloadPlugin(plugins[plugins.GetCount() - 1]);
        }
        plugins.Clear();
    }

    void EditorPluginManager::unloadPlugin(PluginElement* elem)
    {
        if (!elem)
            return;

        Log::getSingleton().logInfo("Unload plugin : file %s, name %s", 
            elem->fileName.ToUTF8().data(), elem->info.name.ToUTF8().data());

        detachPlugin(elem->plugin);

        if (elem->freeProc)
            elem->freeProc(elem->plugin);
        else
            delete elem->plugin;

        wxPluginManager::UnloadLibrary(elem->fileName);

        plugins.Remove(elem);

        delete elem;
    }

    int EditorPluginManager::scanForPlugins(const wxString& path)
    {
        static const wxString pluginsMask = wxT("*.plugin");
        int count = 0;
        if(!wxDirExists(path))
            return count;
        wxDir dir(path);

        if (!dir.IsOpened())
            return count;

        wxString filename;
        wxString failed;
        bool ok = dir.GetFirst(&filename, pluginsMask, wxDIR_FILES);
        while (ok)
        {
            wxFileName fname(path + _T('/') + filename);
            if (loadPlugin(fname.GetFullPath()))
                ++count;
            else
                failed << _T('\n') << filename;

            ok = dir.GetNext(&filename);
        }

        if (!failed.IsEmpty())
        {
            wxTrace(wxT("Failed to load plugins : \n") + failed);
        }
        return count;
    }

    bool EditorPluginManager::loadPlugin(const wxString& fileName)
    {
        registeredPlugins.clear();

        wxPluginLibrary *lib = wxPluginManager::LoadLibrary(fileName, wxDL_VERBATIM);

        if (!lib)
        {
            return false;
        }

        std::vector<PluginRegistration>::iterator it = registeredPlugins.begin();
        for (; it != registeredPlugins.end(); ++it)
        {
            PluginRegistration& pr = *it;
            EditorPlugin* plugin = 0;
            try
            {
                plugin = pr.fncreatePlugin();
            }
            catch (...)
            {
                continue;
            }

            PluginElement* elem = new PluginElement;
            elem->fileName = fileName;
            elem->info = pr.info;
            elem->plugin = plugin;
            elem->freeProc = pr.fnfreePlugin;
            plugins.Add(elem);
        }

        size_t count = registeredPlugins.size();

        if (0 == count)
        {
            wxPluginManager::UnloadLibrary(fileName);
        }
        else
        {
            while (--count)
                lib->RefLib();
        }

        return true;
    }

    void EditorPluginManager::loadAllPlugins()
    {
        PluginElement* elem = 0;
        for (unsigned int i = 0; i < plugins.GetCount(); ++i)
        {
            elem = plugins[i];
            EditorPlugin* plug = elem->plugin;
            if (!plug || plug->isAttached())
                continue;

            attachPlugin(plug);
        }
    }

    void EditorPluginManager::registerPlugin(const PluginInfo& info, FNcreatePlugin fncreatePlugin,
        FNfreePlugin fnfreePlugin, FNgetVersion fngetVersion)
    {
        if (!fncreatePlugin || !fnfreePlugin || !fngetVersion)
            return;

        if (findPluginByName(info.name))
            return;

        int major;
        int minor;
        int release;
        fngetVersion(&major, &minor, &release);
        if (major != PLUGIN_SDK_VERSION_MAJOR ||
            minor != PLUGIN_SDK_VERSION_MINOR ||
            release != PLUGIN_SDK_VERSION_RELEASE)
        {
            Log::getSingleton().logWarning("Failed to register plugin %s, version : editor(%d,%d,%d), plugin(%d,%d,%d)",
                PLUGIN_SDK_VERSION_MAJOR, PLUGIN_SDK_VERSION_MINOR, PLUGIN_SDK_VERSION_RELEASE,
                major, minor, release);
            return;
        }

        PluginRegistration pr;
        pr.info = info;
        pr.fncreatePlugin = fncreatePlugin;
        pr.fnfreePlugin = fnfreePlugin;
        pr.fngetVersion = fngetVersion;
        registeredPlugins.push_back(pr);
    }

    EditorPlugin* EditorPluginManager::findPluginByName(const wxString& pluginName)
    {
        for (unsigned int i = 0; i < plugins.GetCount(); ++i)
        {
            PluginElement* elem = plugins[i];
            if (elem->info.name == pluginName)
                return elem->plugin;
        }
        return 0;
    }

    EditorPlugin* EditorPluginManager::findPluginByFileName(const wxString& pluginFileName)
    {
        for (unsigned int i = 0; i < plugins.GetCount(); ++i)
        {
            PluginElement* elem = plugins[i];
            if (elem->fileName == pluginFileName)
                return elem->plugin;
        }
        return 0;
    }

    PluginsArray EditorPluginManager::findPlugins(EditorPluginType type)
    {
        PluginsArray arr;

        for (unsigned int i = 0; i < plugins.GetCount(); ++i)
        {
            PluginElement* elem = plugins[i];
            if (elem->plugin->getType() == type)
                arr.Add(elem->plugin);
        }
        return arr;
    }

    const PluginInfo* EditorPluginManager::getPluginInfo(EditorPlugin* plugin)
    {
        for (unsigned int i = 0; i < plugins.GetCount(); ++i)
        {
            PluginElement* plugElem = plugins[i];
            if (plugElem->plugin == plugin)
                return &plugElem->info;
        }
        return 0;
    }
}


