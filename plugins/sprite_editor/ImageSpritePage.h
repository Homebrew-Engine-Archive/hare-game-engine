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

class ImageSpritePage : public EditorPage
{
    DECLARE_ABSTRACT_CLASS(ImageSpritePage)
public:
    ImageSpritePage(wxWindow* parent, SpriteMIMEHandler* handler);
    virtual ~ImageSpritePage();

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs();

    friend class SceneListenerEditorWrapper<ImageSpritePage>;
    void renderScene();

    void setImageSprite(ImageSprite* sprite);
    void updateTitle();

protected:
    void onSize(wxSizeEvent& event);
    void onLButtonDown(wxMouseEvent& event);
    void onLButtonUp(wxMouseEvent& event);
    void onMouseMove(wxMouseEvent& event);
private:
    wxPoint mousePos;

protected:
    SpriteMIMEHandler* mime;
    wxHareCanvas* canvas;
    SceneManager* scene;

    ImageSprite::Ptr imageSprite;

    bool isModified;
};

#endif