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

namespace hare_editor
{
    class EDITOR_API EditorEvent : public wxCommandEvent
    {
    public:
        EditorEvent(wxEventType commandType = wxEVT_NULL, int id = 0)
            : wxCommandEvent(commandType, id), editorPage(0), plugin(0)
        {
        }

        EditorEvent(const EditorEvent& rhs)
            : wxCommandEvent(rhs), editorPage(rhs.editorPage), plugin(rhs.plugin)
        {
        }

        virtual wxEvent *Clone() const 
        { 
            return new EditorEvent(*this); 
        }

        EditorPage* editorPage;
        EditorPlugin* plugin;
        DECLARE_DYNAMIC_CLASS(EditorEvent)
    };

    typedef void (wxEvtHandler::*EditorEventFunction)(EditorEvent&);

    extern EDITOR_API const wxEventType editorEVT_EDITOR_UPDATE_UI;
    extern EDITOR_API const wxEventType editorEVT_PLUGIN_ATTACHED;
    extern EDITOR_API const wxEventType editorEVT_PLUGIN_DETACHED;
}

#endif
