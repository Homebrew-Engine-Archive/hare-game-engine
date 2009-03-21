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
    
    canvsText = new wxHareCanvas(splitter);
    canvsCache = new wxHareCanvas(splitter);

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
    changeFont(NULL);
}

void FontEditorPage::onTextUpdate(wxCommandEvent& event)
{
    txtListener.text = txtSample->GetValue().ToUTF8().data();
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
        SimpleShader* shader = new SimpleShader();
        TextureMtrl* texMtrl = new TextureMtrl();
        texMtrl->setTexture(fontPtr->getFontTexture());
        shader->setShaderParams(font->getFontExtParams());
        shader->setMaterial(texMtrl);
        
        txtListener.font = fontPtr;
        cacheListener.cacheTex = shader;
    }
    
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("FontProperity"), fontPtr);

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
