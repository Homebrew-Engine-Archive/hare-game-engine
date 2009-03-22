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
        wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
    splitter->SetMinimumPaneSize(50);
    
    canvsText = new wxHareCanvas(splitter, idFontTextWindow);
    canvsText->Connect(wxEVT_SIZE, wxSizeEventHandler(FontEditorPage::onSize), NULL, this);
    canvsCache = new wxHareCanvas(splitter, idFontCacheWindow);
    canvsCache->Connect(wxEVT_SIZE, wxSizeEventHandler(FontEditorPage::onSize), NULL, this);

    splitter->SplitHorizontally(canvsText, canvsCache, 0);
    sizer1->Add(splitter, 1, wxEXPAND, 5);

    SetSizer(sizer1);
    Layout();

    sceneText = getHareApp()->createSceneManager();
    sceneCache = getHareApp()->createSceneManager();

    sceneText->setSceneListener(&txtListener);
    sceneCache->setSceneListener(&cacheListener);

    canvsText->getRenderWindow()->setSceneManager(sceneText);
    canvsCache->getRenderWindow()->setSceneManager(sceneCache);

    changeFont(font);
}

FontEditorPage::~FontEditorPage()
{
    if (fontPtr)
    {
        fontPtr->saveToXml(fontPtr->getUrl());
        fontPtr = 0;
    }

    mime->page = NULL;
}

void FontEditorPage::onTextUpdate(wxCommandEvent& event)
{
    txtListener.text = txtSample->GetValue().ToUTF8().data();
}

void FontEditorPage::onSize(wxSizeEvent& event)
{
    if (event.GetSize().GetWidth() > 0 && event.GetSize().GetHeight() > 0)
    {
        if (event.GetId() == idFontTextWindow)
            canvsText->getRenderWindow()->resize(event.GetSize().GetWidth(), event.GetSize().GetHeight());
        else if (event.GetId() == idFontCacheWindow)
            canvsCache->getRenderWindow()->resize(event.GetSize().GetWidth(), event.GetSize().GetHeight());
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
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("FontProperity"), fontPtr);

    setTitle(wxT("[FontEditor]") + wxString::FromUTF8(fontPtr->getUrl().c_str()));

    return true;
}

FontMIMEHandler::FontMIMEHandler()
{
    page = new FontEditorPage(Manager::getInstancePtr()->getEditorPageManager()->getNotebook(), this, NULL);
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
        page = new FontEditorPage(Manager::getInstancePtr()->getEditorPageManager()->getNotebook(), this, font);
    else
        page->changeFont(font);

    Manager::getInstancePtr()->getEditorPageManager()->addEditorPage(page);

    return page->isOk();
}
