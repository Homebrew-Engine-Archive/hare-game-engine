//***************************************************************
//  File:    EditorPlugin.cpp
//  Data:    11/03/2008
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
#include "EditorPlugin.h"

namespace hare
{
    EditorPlugin::EditorPlugin() : attached(false), type(EPT_Unknown)
    {
    }

    EditorPlugin::~EditorPlugin()
    {
    }

    void EditorPlugin::attach()
    {
        if (attached)
            return;

        wxWindow* window = Manager::getInstancePtr()->getAppWindow();
        if (window)
        {
            window->PushEventHandler(this);
        }
        attached = true;
        onAttach();
        SetEvtHandlerEnabled(true);

        EditorEvent event(editorEVT_PLUGIN_ATTACHED);
        event.plugin = this;
        Manager::getInstancePtr()->processEvent(event);
    }

    void EditorPlugin::detach(bool isShutDown)
    {
        if (!attached)
            return;

        attached = false;
        SetEvtHandlerEnabled(false);
        onDetach(isShutDown);

        EditorEvent event(editorEVT_PLUGIN_DETACHED);
        event.plugin = this;
        Manager::getInstancePtr()->processEvent(event);

        wxWindow* window = Manager::getInstancePtr()->getAppWindow();
        if (window)
        {
            window->RemoveEventHandler(this);
        }
    }
}
