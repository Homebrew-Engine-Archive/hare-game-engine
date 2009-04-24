//***************************************************************
//  File:    EditorEvent.cpp
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
#include "PCH.h"
#include "EditorEvent.h"

namespace hare
{
    IMPLEMENT_DYNAMIC_CLASS(EditorEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(EditorDockEvent, wxEvent)

    const wxEventType editorEVT_APP_BEFORE_SHUTDOWN = wxNewEventType();
    
    const wxEventType editorEVT_PLUGIN_ATTACHED = wxNewEventType();
    const wxEventType editorEVT_PLUGIN_DETACHED = wxNewEventType();

    const wxEventType editorEVT_PROJECT_ACTIVED = wxNewEventType();
    
    const wxEventType editorEVT_LAYOUT_SWITCH = wxNewEventType();
    const wxEventType editorEVT_LAYOUT_QUERY = wxNewEventType();

    const wxEventType editorEVT_EDITOR_UPDATE_UI = wxNewEventType();
    const wxEventType editorEVT_EDITOR_ACTIVATED = wxNewEventType();
    const wxEventType editorEVT_EDITOR_DEACTIVATED = wxNewEventType();
    const wxEventType editorEVT_EDITOR_CLOSE = wxNewEventType();
    
    const wxEventType editorEVT_ADD_DOCK_WINDOW = wxNewEventType();
    const wxEventType editorEVT_DEL_DOCK_WINDOW = wxNewEventType();
    const wxEventType editorEVT_SHOW_DOCK_WINDOW = wxNewEventType();
}

