//***************************************************************
//  File:    ImageSpritePage.h
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
#ifndef _IMAGESPRITEPAGE_H_
#define _IMAGESPRITEPAGE_H_

#include <wx/harecanvas.h>

class SpriteMIMEHandler;

class ImageSpritePage : public EditorPage, public SceneListenerBase
{
    DECLARE_ABSTRACT_CLASS(ImageSpritePage)
public:
    ImageSpritePage(wxWindow* parent, SpriteMIMEHandler* handler);
    virtual ~ImageSpritePage();

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();

    virtual void beginScene();
    virtual void endScene();
    virtual void renderScene();

    //void add

protected:
    SpriteMIMEHandler* mime;
    wxHareCanvas* canvas;
    SceneManager* scene;

    bool isModified;
};

#endif