//***************************************************************
//  File:    EditorPlugin.h
//  Data:    11/02/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _EDITORPLUGIN_H_
#define _EDITORPLUGIN_H_

#include "EditorPrerequisites.h"
#include "EditorManager.h"

namespace hare_editor
{
    enum EditorPluginType
    {
        EPT_Unknown,
        EPT_Compiler,
        EPT_Debugger,
        EPT_Wizard,
        EPT_MIMEHandler,
        EPT_Tool,
    };

    class EDITOR_API EditorPlugin : public wxEvtHandler
    {
    public:
        EditorPlugin();
        virtual ~EditorPlugin();

        virtual EditorPluginType getType() 
        { 
            return type; 
        }

        virtual bool buildMenuBar(wxMenuBar* menuBar) = 0;
        virtual bool buildToolBar(wxAuiToolBar* toolBar) = 0;

        virtual bool canDetach()
        {
            return true;
        }

        bool isAttached()
        {
            return attached;
        }
    protected:
        virtual void onAttach()
        {
        }
        virtual void onDetach(bool isShutDown)
        {
        }

    protected:
        EditorPluginType type;
        bool attached;

    private:
        friend class PluginManager;
        void attach();
        void detach(bool isShutDown);

    };
}

#endif
