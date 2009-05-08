//***************************************************************
//  File:    GUIMIMEHandler.cpp
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
#include "PCH.h"
#include "GUIMIMEHandler.h"
#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

IMPLEMENT_ABSTRACT_CLASS(GUIEditorPageImpl, EditorPage)

GUIEditorPageImpl::GUIEditorPageImpl(wxWindow* parent, GUIMIMEHandler* handler)
 : GUIEditorPage(parent), mime(handler)
{
}

GUIEditorPageImpl::~GUIEditorPageImpl()
{
   
}

GUIMIMEHandler::GUIMIMEHandler()
{
}

bool GUIMIMEHandler::newPage(Window* window, bool isModified)
{
    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    epm->getNotebook()->Freeze();
    GUIEditorPageImpl* page = new GUIEditorPageImpl(epm->getNotebook(), this);
    epm->addEditorPage(page);
    epm->getNotebook()->Thaw();

    //page->setWindow(window);

    if (isModified)
        page->setModified(isModified);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return true;
}

bool GUIMIMEHandler::canHandle(const wxString& filename) const
{
    return filename.Lower().EndsWith(wxT(".layout"));
}

bool GUIMIMEHandler::openFile(const wxString& filename)
{
    Window::Ptr window = (Window*)Object::importObject(filename.ToUTF8().data());

    //if (!window)
    //    return false;

    return newPage(window, false);
}
