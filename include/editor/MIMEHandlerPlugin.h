//***************************************************************
//  File:    MIMEHandlerPlugin.h
//  Data:    01/27/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef MIMEHANDLERPLUGIN_H
#define MIMEHANDLERPLUGIN_H

#include "EditorPrerequisites.h"
#include "EditorPlugin.h"

namespace hare
{
    class EDITOR_API MIMEHandlerPlugin : public EditorPlugin
    {
    public:
        MIMEHandlerPlugin();
        virtual ~MIMEHandlerPlugin();

    public:
        virtual bool canHandle(const wxString& filename) const = 0;

        virtual bool openFile(const wxString& filename) = 0;

    private:
        virtual bool buildMenuBar(wxMenuBar* menuBar)
        {
            return false;
        }
        virtual bool buildToolBar(wxAuiToolBar* toolBar)
        {
            return false;
        }
    };
}

#endif
