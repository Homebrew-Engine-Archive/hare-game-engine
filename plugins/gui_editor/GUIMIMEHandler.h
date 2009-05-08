//***************************************************************
//  File:    GUIMIMEHandler.h
//  Data:    05/08/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef GUIMIMEHANDLER_H
#define GUIMIMEHANDLER_H

#include "GUIEditorPage.h"
#include <wx/harecanvas.h>

class GUIMIMEHandler;

class GUIEditorPageImpl : public GUIEditorPage 
{
public:
    GUIEditorPageImpl(wxWindow* parent, GUIMIMEHandler* handler);
    virtual ~GUIEditorPageImpl(); 

private:
    GUIMIMEHandler* mime;

private:
    DECLARE_ABSTRACT_CLASS(GUIEditorPageImpl)
};

class GUIMIMEHandler : public MIMEHandlerPlugin
{
public:
    GUIMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

    bool newPage(Window* window, bool isModified);
};

#endif