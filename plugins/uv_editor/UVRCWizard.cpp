//***************************************************************
//  File:    UVRCWizard.cpp
//  Data:    05/06/2009
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
#include "UVRCWizard.h"
#include "UVRCMIMEHandler.h"

UVRCWizard::UVRCWizard()
{
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
    bitmap.LoadFile(fullPath + wxT("file32.png"), wxBITMAP_TYPE_PNG);
}

wxString UVRCWizard::getFolder() const
{
    return wxT("UVResource");
}

const wxBitmap& UVRCWizard::getBitmap(int index) const
{
    return bitmap;
}

wxString UVRCWizard::getTitle(int index) const
{
    return wxT("UVResource");
}

wxString UVRCWizard::getDesc(int index) const
{
    return wxT("Create UVResource");
}

int UVRCWizard::getCount() const
{
    return 1;
}

Object* UVRCWizard::wizard(int index)
{
    UVEditorState::Ptr object = (UVEditorState*)UVEditorState::CLASS_INFO.createObject();

    if (object)
    {
        EditorPlugin* plugin = PluginManager::getInstancePtr()->findPluginByName(wxT("UVRCMIMEHandler"));
        if (plugin && plugin->getType() == EPT_MIMEHandler)
        {
            UVRCMIMEHandler* handler = (UVRCMIMEHandler*)plugin;
            handler->newPage(object);
        }
    }

    return NULL;
}