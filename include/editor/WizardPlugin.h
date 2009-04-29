//***************************************************************
//  File:    WizardPlugin.h
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
#ifndef WIZARDPLUGIN_H
#define WIZARDPLUGIN_H

#include "EditorPrerequisites.h"
#include "EditorPlugin.h"

namespace hare
{
    class EDITOR_API WizardPlugin : public EditorPlugin
    {
    public:
        WizardPlugin();
        virtual ~WizardPlugin();

    public:
        virtual wxString getFolder() const = 0;
        virtual const wxBitmap& getBitmap(int index) const = 0;
        virtual wxString getTitle(int index) const = 0;
        virtual wxString getDesc(int index) const = 0;
        virtual int getCount() const = 0;
        virtual Object* wizard(int index) = 0;

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
