//***************************************************************
//  File:    ComponentSpritePage.h
//  Data:    05/17/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef _COMPONENTSSPRITEPAGE_H_
#define _COMPONENTSSPRITEPAGE_H_

#include <wx/harecanvas.h>


class SpriteMIMEHandler;

class ComponentSpritePage : public EditorPage, public SceneListenerBase
{
    DECLARE_ABSTRACT_CLASS(ComponentSpritePage)
public:
    ComponentSpritePage(wxWindow* parent, SpriteMIMEHandler* handler);
    virtual ~ComponentSpritePage();

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs();
    void updateTitle();

    virtual void beginScene();
    virtual void endScene();
    virtual void renderScene();

    void setComponentSprite(ComponentSprite* sprite);
    void setCurPartSprite(Sprite* sprite);
    void addPartSprite(Sprite* sprite);

protected:
    bool isModified;
    SpriteMIMEHandler* mime;
    ComponentSprite::Ptr componentSprite;
    Sprite::Ptr curPartSprite;

    wxHareCanvas* componentCanvas;
    wxHareCanvas* partSpriteCanvas;
    SceneManager* componentScene;
    SceneManager* partSpriteScene;

    wxPoint componentMousePos;
    wxPoint partSpriteMousePos;

    wxTreeItemId rightMouseHitItem;
    wxTreeItemId dragTreeItem;
    
protected:
    wxTreeCtrl* treeCtrl;
    wxPanel*    componentReview;
    wxPanel*    partSpriteReview;

protected:
    void onBeginDrag(wxTreeEvent& event);
    void onEndDrag(wxTreeEvent& event);
    void onSelChanged(wxTreeEvent& event);
    void onItemRClick(wxTreeEvent& event);
    void onRMouseDown(wxMouseEvent& event);


    void onComponentSize(wxSizeEvent& event);
    void onComponentLButtonDown(wxMouseEvent& event);
    void onComponentLButtonUp(wxMouseEvent& event);
    void onComponentMouseMove(wxMouseEvent& event);

    void onPartSpriteSize(wxSizeEvent& event);
    void onPartSpriteLButtonDown(wxMouseEvent& event);
    void onPartSpriteLButtonUp(wxMouseEvent& event);
    void onPartSpriteMouseMove(wxMouseEvent& event);

    void onEraseBackground(wxEraseEvent& event);
    void onMenuSelected(wxCommandEvent& event);
};

#endif