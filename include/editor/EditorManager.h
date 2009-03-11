//***************************************************************
//  File:    EditorManager.h
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
#ifndef MANAGER_H
#define MANAGER_H

#include "EditorPrerequisites.h"
#include "TManager.h"
#include "EditorEvent.h"

namespace hare_editor
{
    template<typename EventType> 
    class IEventHandlerBase
    {
    public:
        virtual void call(EventType& event) = 0;
        virtual ~IEventHandlerBase(){}
        virtual void* getClass() = 0;
    };

    template<class ClassType, typename EventType> 
    class TEventHandler : public IEventHandlerBase<EventType>
    {
    private:
        typedef void (ClassType::*EventFunc)(EventType&);
        ClassType* cls;
        EventFunc fun;
    public:
        TEventHandler(ClassType* c, EventFunc f) : cls(c), fun(f) {}
        TEventHandler(const TEventHandler<ClassType, EventType>& rhs) : cls(rhs.cls), fun(rhs.fun) {}
        virtual void* getClass() { return cls; }
        virtual void call(EventType& event) { if (cls) (cls->*fun)(event); }
    };

    class EDITOR_API Manager : public TManager<Manager>
    {
        friend class TManager<Manager>;

    public:
        wxFrame* getAppWindow() const;
        wxFrame* setAppWindow(wxFrame* win);
        const wxString getAppDir() const;
        void setAppDir(const wxString& dir);

        static bool isAppShuttingDown()
        {
            return appShuttingDown;
        }

        ExplorerManager* getExplorerManager() const;
        ConfigManager* getConfigManager() const;
        EditorPageManager* getEditorPageManager() const;
        PluginManager* getPluginManager() const;

        bool processEvent(EditorEvent& event);

        void registerEvent(wxEventType eventType, IEventHandlerBase<EditorEvent>* handler);

        void removeAllEventSinksFor(void* owner);

    private:
        wxFrame* appWindow;
        wxString appDir;

        static bool appShuttingDown;

        typedef std::vector<IEventHandlerBase<EditorEvent>*> EventSinksArray;
        typedef std::map<wxEventType, EventSinksArray> EventSinksMap;

        EventSinksMap eventSinks;

    private:
        Manager();
       ~Manager();
    };
}

#endif
