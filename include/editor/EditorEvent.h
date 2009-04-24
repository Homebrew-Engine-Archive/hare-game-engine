//***************************************************************
//  File:    EditorEvent.h
//  Data:    01/23/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef EDITOREVENT_H
#define EDITOREVENT_H

#include "EditorPrerequisites.h"
#include <wx/aui/aui.h>

namespace hare
{
    class EDITOR_API EditorEvent : public wxCommandEvent
    {
    public:
        EditorEvent(wxEventType commandType = wxEVT_NULL, int id = 0)
            : wxCommandEvent(commandType, id), editorPage(0), plugin(0)
        {
        }

        EditorEvent(const EditorEvent& rhs)
            : wxCommandEvent(rhs), editorPage(rhs.editorPage),
            plugin(rhs.plugin), strData(rhs.strData)
        {
        }

        virtual wxEvent *Clone() const
        {
            return new EditorEvent(*this);
        }

        EditorPage* editorPage;
        EditorPlugin* plugin;
        Project* project;
        wxString strData;
        DECLARE_DYNAMIC_CLASS(EditorEvent)
    };

    class EDITOR_API EditorDockEvent : public wxCommandEvent
    {
    public:
        EditorDockEvent(wxEventType commandType = wxEVT_NULL, int id = 0)
            : wxCommandEvent(commandType, id), window(0), show(true)
        {
        }

        EditorDockEvent(const EditorDockEvent& rhs)
            : wxCommandEvent(rhs), window(rhs.window), info(rhs.info), show(rhs.show)
        {
        }

        virtual wxEvent *Clone() const
        {
            return new EditorDockEvent(*this);
        }

        wxWindow* window;
        wxAuiPaneInfo info;
        bool show;
        DECLARE_DYNAMIC_CLASS(EditorDockEvent)
    };

    typedef void (wxEvtHandler::*EditorEventFunction)(EditorEvent&);

    extern EDITOR_API const wxEventType editorEVT_APP_BEFORE_SHUTDOWN;
    
    extern EDITOR_API const wxEventType editorEVT_EDITOR_UPDATE_UI;
    extern EDITOR_API const wxEventType editorEVT_EDITOR_ACTIVATED;
    extern EDITOR_API const wxEventType editorEVT_EDITOR_DEACTIVATED;
    extern EDITOR_API const wxEventType editorEVT_EDITOR_CLOSE;
    
    extern EDITOR_API const wxEventType editorEVT_PLUGIN_ATTACHED;
    extern EDITOR_API const wxEventType editorEVT_PLUGIN_DETACHED;
    
    extern EDITOR_API const wxEventType editorEVT_PROJECT_ACTIVED;
    
    extern EDITOR_API const wxEventType editorEVT_LAYOUT_SWITCH;
    extern EDITOR_API const wxEventType editorEVT_LAYOUT_QUERY;

    extern EDITOR_API const wxEventType editorEVT_ADD_DOCK_WINDOW;
    extern EDITOR_API const wxEventType editorEVT_DEL_DOCK_WINDOW;
    extern EDITOR_API const wxEventType editorEVT_SHOW_DOCK_WINDOW;

}

#endif
