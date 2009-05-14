//***************************************************************
//  File:    AnimationSpritePage.h
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
#ifndef _ANIMATIONSPRITEPAGE_H_
#define _ANIMATIONSPRITEPAGE_H_
#include <wx/harecanvas.h>

class SpriteMIMEHandler;

class AnimationSpritePage : public EditorPage, public SceneListenerBase
{
    DECLARE_ABSTRACT_CLASS(AnimationSpritePage)
public:
    AnimationSpritePage(wxWindow* parent, SpriteMIMEHandler* handler);
    ~AnimationSpritePage();
    
    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs();

    virtual void beginScene();
    virtual void endScene();
    virtual void renderScene();

    void setAnimationSprite(AnimationSprite* sprite);
    void addAnimationFrame(AnimFrame* frame);
    void updateTitle();

protected:
    wxTreeCtrl* treeCtrl;
    wxPanel*    animationReview;
    wxPanel*    frameReview;

    wxTreeItemId treeRootID;

    SpriteMIMEHandler* mime;

    wxHareCanvas* animationCanvas;
    wxHareCanvas* frameCanvas;
    SceneManager* animationScene;
    SceneManager* frameScene;

    AnimationSprite::Ptr animationSprite;

    bool isModified;

protected:
    void onBeginDrag(wxTreeEvent& event);
    void onEndDrag(wxTreeEvent& event);
    void onRMouseDown(wxMouseEvent& event);

    void onAnimationSize(wxSizeEvent& event);
    void onAnimationLButtonDown(wxMouseEvent& event);
    void onAnimationLButtonUp(wxMouseEvent& event);
    void onAnimationMouseMove(wxMouseEvent& event);

    void onFrameSize(wxSizeEvent& event);
    void onFrameLButtonDown(wxMouseEvent& event);
    void onFrameLButtonUp(wxMouseEvent& event);
    void onFrameMouseMove(wxMouseEvent& event);

    void onEraseBackground(wxEraseEvent& event);
    void onMenuSelected(wxCommandEvent& event);
};

#endif