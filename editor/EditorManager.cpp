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
#include <wx/xrc/xmlres.h>
#include <wx/filesys.h>
#include <wx/file.h>
#include <wx/fs_zip.h>
#include <wx/fs_mem.h>
#include "EditorManager.h"
#include "ExplorerManager.h"
#include "ConfigManager.h"
#include "EditorPageManager.h"
#include "EditorPluginManager.h"

namespace hare
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
    }

    void Manager::shutdown()
    {
        appShuttingDown = true;

        ExplorerManager::free();
        ConfigManager::free();
        EditorPageManager::free();
        EditorPluginManager::free();
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

    const wxString Manager::getAppDir() const
    {
        return appDir;
    }

    bool Manager::loadResource(const wxString& fileName)
    {
        wxString resFile = Manager::getInstancePtr()->getAppDir() + wxT("/") + fileName;
        wxString memFile = wxT("memory:") + fileName;

        if (!wxFile::Access(resFile, wxFile::read))
            return false;

        wxFile file(resFile, wxFile::read);
        char *buf = 0;

        try
        {
            size_t len = file.Length();
            buf = new char[len];
            file.Read(buf, len);
            {
                wxMemoryFSHandler::AddFile(fileName, buf, len);
            }
            wxXmlResource::Get()->Load(memFile);
            delete[] buf;
            return false;
        }
        catch (...)
        {
            delete[] buf;
            return true;
        }
    }

    wxBitmap Manager::loadBitmap(const wxString& fileName, int bitmapType)
    {
        wxImage im;
        wxFileSystem fs;
        wxFSFile* f = fs.OpenFile(fileName);
        if (f)
        {
            wxInputStream* is = f->GetStream();
            im.LoadFile(*is, bitmapType);
            delete f;
        }
        return wxBitmap(im);
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

    EditorPluginManager* Manager::getPluginManager() const
    {
        return EditorPluginManager::getInstancePtr();
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
