//***************************************************************
//  File:    FontMIMEHandler.cpp
//  Data:    03/20/2009
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
#include "FontMIMEHandler.h"
#include <wx/splitter.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

int idFontTextWindow = wxNewId();
int idFontCacheWindow = wxNewId();

IMPLEMENT_ABSTRACT_CLASS(FontEditorPage, EditorPage)

FontEditorPage::FontEditorPage(wxWindow* parent, FontMIMEHandler* handler, Font* font)
 : EditorPage(parent), mime(handler)
{
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* txt = new wxStaticText(this, wxID_ANY, wxT("SampleText :"), 
        wxDefaultPosition, wxDefaultSize, 0);
    txt->Wrap(-1);
    sizer2->Add(txt, 0, wxALL, 5);

    txtSample = new wxTextCtrl(this, wxID_ANY, wxEmptyString, 
        wxDefaultPosition, wxDefaultSize, 0);

    txtSample->Connect(wxEVT_COMMAND_TEXT_UPDATED, 
        wxCommandEventHandler(FontEditorPage::onTextUpdate), NULL, this);

    sizer2->Add(txtSample, 1, 0, 5);

    sizer1->Add(sizer2, 0, wxEXPAND, 5);

    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, 
        wxDefaultPosition, wxDefaultSize, wxSP_3D);
    splitter->SetMinimumPaneSize(50);
    
    canvasText = new wxHareCanvas(splitter, idFontTextWindow);
    canvasText->Connect(wxEVT_SIZE, wxSizeEventHandler(FontEditorPage::onSize), NULL, this);
    canvasCache = new wxHareCanvas(splitter, idFontCacheWindow);
    canvasCache->Connect(wxEVT_SIZE, wxSizeEventHandler(FontEditorPage::onSize), NULL, this);

    splitter->SplitHorizontally(canvasText, canvasCache, 150);
    sizer1->Add(splitter, 1, wxEXPAND, 5);

    SetSizer(sizer1);
    Layout();

    sceneText = getHareApp()->createSceneManager();
    sceneCache = getHareApp()->createSceneManager();

    sceneText->setSceneListener(&txtListener);
    sceneCache->setSceneListener(&cacheListener);

    canvasText->getRenderWindow()->setSceneManager(sceneText);
    canvasCache->getRenderWindow()->setSceneManager(sceneCache);

    changeFont(font);
}

FontEditorPage::~FontEditorPage()
{
    if (fontPtr)
    {
        fontPtr->saveToXml(fontPtr->getUrl());
        fontPtr = 0;

        if (!Manager::isAppShuttingDown())
        {
            Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        }
    }

    mime->page = NULL;
}

bool FontEditorPage::Show(bool show)
{
    if (show)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        if (fontPtr)
            Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("FontProperity"), fontPtr, this);
    }
    return EditorPage::Show(show);
}

void FontEditorPage::onTextUpdate(wxCommandEvent& event)
{
    txtListener.text = txtSample->GetValue().ToUTF8().data();
}

void FontEditorPage::onSize(wxSizeEvent& event)
{
    if (event.GetId() == idFontTextWindow)
	{
        wxSize size = canvasText->GetClientSize();
        if (size.GetWidth() > 0 && size.GetHeight() > 0)
            canvasText->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
    }
    else if (event.GetId() == idFontCacheWindow)
    {
        wxSize size = canvasCache->GetClientSize();
        if (size.GetWidth() > 0 && size.GetHeight() > 0)
            canvasCache->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
    }
}


bool FontEditorPage::changeFont(Font* font)
{
    if (font == fontPtr)
        return true;

    if (fontPtr)
    {
        fontPtr->saveToXml(fontPtr->getUrl());
    }

    fontPtr = font;

    if (fontPtr)
    {
        txtListener.font = fontPtr;
        cacheListener.font = fontPtr;
    }
    
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("FontProperity"), fontPtr, this);

    setTitle(wxT("[FontEditor]") + wxString::FromUTF8(fontPtr->getUrl().c_str()));

    return true;
}

FontMIMEHandler::FontMIMEHandler() : page(0)
{
}

bool FontMIMEHandler::canHandle(const wxString& filename) const
{
    return filename.Lower().EndsWith(wxT(".font"));
}

bool FontMIMEHandler::openFile(const wxString& filename)
{
    Font::Ptr font = (Font*)Object::importObject(filename.ToUTF8().data());

    if (!font)
        return false;

    if (!page)
    {
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Freeze();
        page = new FontEditorPage(Manager::getInstancePtr()->getEditorPageManager()->getNotebook(), this, font);
        Manager::getInstancePtr()->getEditorPageManager()->addEditorPage(page);
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Thaw();
    }
    else
        page->changeFont(font);

    return page->isOk();
}
