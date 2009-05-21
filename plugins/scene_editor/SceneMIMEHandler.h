//***************************************************************
//  File:    SceneMIMEHandler.h
//  Date:    5/19/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _SCENEMIMEHANDLER_H_
#define _SCENEMIMEHANDLER_H_

#include <wx/harecanvas.h>
#include <wx/tglbtn.h>

class SceneMIMEHandler;

class ScenePage : public EditorPage, public SceneListenerBase
{
    DECLARE_ABSTRACT_CLASS(ImageSpritePage)
public:
    ScenePage(wxWindow* parent, SceneMIMEHandler* handler, SceneManager* s);
    virtual ~ScenePage();

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs();

    virtual void beginScene();
    virtual void endScene();
    virtual void renderScene();
    void updateTitle();

protected:
    bool isModified;

    wxHareCanvas* spriteCanvas;
    wxHareCanvas* sceneCanvas;
    SceneManager::Ptr sceneManager;
    SceneManager*     spriteScene;
    SceneMIMEHandler* mime;

    Sprite::Ptr curAddSprite;
    Sprite::Ptr curEditSprite;

    wxTreeCtrl*     sceneTree;
    wxPanel*        scenePanel;
    wxPanel*        spritePanel;
    wxListBox*      spriteListCtrl;
    wxToggleButton* btnAddSprite;

    Font::Ptr font;

    wxPoint rightDownPos;
    wxCursor *dragCursor;

protected:
    void onTreeItemSelected(wxTreeEvent& event);

    void onSceneSize(wxSizeEvent& event);
    void onSceneLButtonDown(wxMouseEvent& event);
    void onSceneLButtonUp(wxMouseEvent& event);
    void onSceneMouseMove(wxMouseEvent& event);

    void onSceneRButtonDown(wxMouseEvent& event);
    void onSceneRButtonUp(wxMouseEvent& event);

    void onSpriteSceneSize(wxSizeEvent& event);
    void onListBoxItemSelected(wxCommandEvent& event);

    void onEraseBackground(wxEraseEvent& event);

public:
    void addSpriteToScene(Sprite* sprite);
};


class SceneMIMEHandler : public MIMEHandlerPlugin
{
public:
    SceneMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

    bool newPageScene(SceneManager* scene, bool isModified = true);
};

#endif
