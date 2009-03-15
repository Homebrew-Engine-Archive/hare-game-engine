//***************************************************************
//  File:    EditorManager.cpp
//  Date:    10/23/2008
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
#include "EditorManager.h"
#include "ExplorerManager.h"
#include "ConfigManager.h"
#include "EditorPageManager.h"
#include "PluginManager.h"

namespace hare_editor
{
    template<> Manager* TManager<Manager>::instance = 0;
    template<> bool TManager<Manager>::autoCreate = true;
    bool Manager::appShuttingDown = false;

    Manager::Manager() : appWindow(0)
    {
        appDir = wxString::FromUTF8(FileSystem::getSingletonPtr()->getBaseDir());
    }

    Manager::~Manager()
    {
        for (EventSinksMap::iterator mit = eventSinks.begin(); mit != eventSinks.end(); ++mit)
        {
            EventSinksArray::iterator it = mit->second.begin();
            while (mit->second.size())
            {
                delete (*(mit->second.begin()));
                mit->second.erase(mit->second.begin());
            }
        }

        for (DockEventSinksMap::iterator mit = dockEventSinks.begin(); mit != dockEventSinks.end(); ++mit)
        {
            DockEventSinksArray::iterator it = mit->second.begin();
            while (mit->second.size())
            {
                delete (*(mit->second.begin()));
                mit->second.erase(mit->second.begin());
            }
        }

        appShuttingDown = true;

        ExplorerManager::free();
        ConfigManager::free();
        EditorPageManager::free();
        PluginManager::free();
    }

    wxFrame* Manager::getAppWindow() const
    {
        return appWindow;
    }

    wxFrame* Manager::setAppWindow(wxFrame* win)
    {
        assert(appWindow == 0);

        appWindow = win;

        return appWindow;
    }

    void Manager::setAppDir(const wxString& dir)
    {
        appDir = dir;
    }

    const wxString Manager::getAppDir() const
    {
        return appDir;
    }

    ExplorerManager* Manager::getExplorerManager() const
    {
        return ExplorerManager::getInstancePtr();
    }

    ConfigManager* Manager::getConfigManager() const
    {
        return ConfigManager::getInstancePtr();
    }

    EditorPageManager* Manager::getEditorPageManager() const
    {
        return EditorPageManager::getInstancePtr();
    }

    PluginManager* Manager::getPluginManager() const
    {
        return PluginManager::getInstancePtr();
    }

    bool Manager::processEvent(EditorEvent& event)
    {
        if (isAppShuttingDown())
            return false;

        EventSinksMap::iterator it = eventSinks.find(event.GetEventType());
        if (it != eventSinks.end())
        {
            EventSinksArray::iterator ite = it->second.begin();
            for (; ite != it->second.end(); ++ite)
            {
                (*ite)->call(event);
            }
        }
        return true;
    }

    bool Manager::processEvent(EditorDockEvent& event)
    {
        if (isAppShuttingDown())
            return false;

        DockEventSinksMap::iterator it = dockEventSinks.find(event.GetEventType());
        if (it != dockEventSinks.end())
        {
            DockEventSinksArray::iterator ite = it->second.begin();
            for (; ite != it->second.end(); ++ite)
            {
                (*ite)->call(event);
            }
        }
        return true;
    }

    void Manager::registerEvent(wxEventType eventType, IEventHandlerBase<EditorEvent>* handler)
    {
        eventSinks[eventType].push_back(handler);
    }

    void Manager::registerEvent(wxEventType eventType, IEventHandlerBase<EditorDockEvent>* handler)
    {
        dockEventSinks[eventType].push_back(handler);
    }

    void Manager::removeAllEventSinksFor(void* owner)
    {
        for (EventSinksMap::iterator mit = eventSinks.begin(); mit != eventSinks.end(); ++mit)
        {
            EventSinksArray::iterator it = mit->second.begin();
            bool endIsInvalid = false;
            while (!endIsInvalid && it != mit->second.end())
            {
                if ((*it) && (*it)->getClass() == owner)
                {
                    EventSinksArray::iterator it2 = it++;
                    endIsInvalid = it == mit->second.end();
                    delete (*it2);
                    mit->second.erase(it2);
                }
                else
                    ++it;
            }
        }

        for (DockEventSinksMap::iterator mit = dockEventSinks.begin(); mit != dockEventSinks.end(); ++mit)
        {
            DockEventSinksArray::iterator it = mit->second.begin();
            bool endIsInvalid = false;
            while (!endIsInvalid && it != mit->second.end())
            {
                if ((*it) && (*it)->getClass() == owner)
                {
                    DockEventSinksArray::iterator it2 = it++;
                    endIsInvalid = it == mit->second.end();
                    delete (*it2);
                    mit->second.erase(it2);
                }
                else
                    ++it;
            }
        }
    }
}
