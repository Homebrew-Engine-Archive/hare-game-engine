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

namespace hare_editor
{
    IMPLEMENT_DYNAMIC_CLASS(EditorEvent, wxEvent)

    const wxEventType editorEVT_EDITOR_UPDATE_UI = wxNewEventType();
    const wxEventType editorEVT_PLUGIN_ATTACHED = wxNewEventType();
    const wxEventType editorEVT_PLUGIN_DETACHED = wxNewEventType();
}

