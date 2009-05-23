//***************************************************************
//  File:    ImageSpritePage.cpp
//  Date:    5/11/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "ImageSpritePage.h"


IMPLEMENT_ABSTRACT_CLASS(ImageSpritePage, EditorPage)

ImageSpritePage::ImageSpritePage(wxWindow* parent, SpriteMIMEHandler* handler)
    :EditorPage(parent)
    ,mime(handler)
    ,isModified(false)
{
    canvas = new wxHareCanvas(this);
    scene = getHareApp()->createSceneManager();
    scene->setSceneListener(new SceneListenerEditorWrapper<ImageSpritePage>(this));
    canvas->getRenderWindow()->setSceneManager(scene);


    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(canvas, 1, wxEXPAND, 0);
    SetSizer(sizer);
    Layout();

    canvas->Connect(wxEVT_SIZE, wxSizeEventHandler(ImageSpritePage::onSize), 0, this);
    canvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ImageSpritePage::onLButtonDown), 0, this);
    canvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ImageSpritePage::onLButtonUp), 0, this);
    canvas->Connect(wxEVT_MOTION, wxMouseEventHandler(ImageSpritePage::onMouseMove), 0, this);
}

ImageSpritePage::~ImageSpritePage()
{
    if (!Manager::isAppShuttingDown()){
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }
}

void ImageSpritePage::updateTitle()
{
    String title = "[ImageSpriteEditor]";
    if (imageSprite) title += imageSprite->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}

void ImageSpritePage::setModified(bool modified)
{
    if (modified != isModified){
        isModified = modified;
        updateTitle();
    }
}

bool ImageSpritePage::saveAs()
{
    bool ret = false;

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        _T("Save ImageSprite resource as"),
        _T(""),
        _T(""),
        _T("ImageSprite Resource (*.sprite)|*.sprite|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK){
        static const char* tempFileName = "/editor/~sprite.temp";
        FileSystem::getSingletonPtr()->remove(tempFileName);
        imageSprite->saveToXml(tempFileName);
        String dir = FileSystem::getSingletonPtr()->getRealDir(tempFileName);
        dir += tempFileName;
        wxString tempFile = wxString::FromUTF8(dir.c_str());
        wxFileName fName(tempFile);
        if (fName.FileExists()){
            wxString fileName = dlg->GetPath();
            ret = wxCopyFile(fName.GetFullPath(), fileName);
        }
    }

    dlg->Destroy();
    return ret;
}

bool ImageSpritePage::save()
{
    if (!imageSprite)
        return true;

    bool ret = false;
    if (imageSprite->getUrl().empty())
        ret = saveAs();
    else
        ret = imageSprite->saveToXml(imageSprite->getUrl());

    if (ret)
        setModified(false);
        
    return ret;
}

void ImageSpritePage::renderScene()
{
    wxSize size = canvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        uint32 oldColor = getCanvas()->getColor();
        getCanvas()->setColor(0xff00ff00);
        getCanvas()->drawLine(-1000, size.GetHeight()/2, 1000, size.GetHeight()/2);
        getCanvas()->drawLine(size.GetWidth()/2, -1000, size.GetWidth()/2, 1000);
        getCanvas()->setColor(oldColor);
    }

}

void ImageSpritePage::setImageSprite(ImageSprite* sprite)
{
    imageSprite = sprite;
    imageSprite->moveTo(0,0);
    scene->addSprite(imageSprite);
    updateTitle();

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ImageSpriteProperity"), imageSprite);
}

void ImageSpritePage::onSize(wxSizeEvent& event)
{
    wxSize size = canvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        canvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        canvas->getRenderWindow()->moveCameraTo(-size.GetWidth()/2, -size.GetHeight()/2);
    }
    event.Skip();
}

void ImageSpritePage::onLButtonDown(wxMouseEvent& event)
{
    mousePos = event.GetPosition();
}

void ImageSpritePage::onLButtonUp(wxMouseEvent& event)
{
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ImageSpriteProperity"), imageSprite);
}

void ImageSpritePage::onMouseMove(wxMouseEvent& event)
{
    if (!imageSprite)
        return;

    if (event.Dragging() && event.LeftIsDown()){
        wxPoint offset = event.GetPosition() - mousePos;
        mousePos = event.GetPosition();
        imageSprite->moveOrigoPos(offset.x, offset.y);
        setModified(true);
    }
}
