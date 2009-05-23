//***************************************************************
//  File:    ThemeMIMEHandler.h
//  Data:    05/14/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef THEMEMIMEHANDLER_H
#define THEMEMIMEHANDLER_H

#include <wx/harecanvas.h>

class ThemeMIMEHandler;

class TreeItemBase : public wxTreeItemData
{
public:
    enum TreeItemType {Type_Theme, Type_Rect};

    virtual TreeItemType getType() = 0;
};

class PreviewSceneListener : public SceneListener
{
public:
    PreviewSceneListener()
    {
        guiSys = new GUISystem();
    }

    virtual void beginScene()
    {
    }
    virtual void endScene()
    {
    }
    virtual void renderScene()
    {
        guiSys->render();
    }

    GUISystem* getGUISystem() { return guiSys; }

private:
    GUISystem::Ptr guiSys;
};

class ThemeEditorPage : public EditorPage, public SceneListener
{
public:
    ThemeEditorPage(wxWindow* parent, ThemeMIMEHandler* handler);
    virtual ~ThemeEditorPage();

    void setThemePackage(ThemePackage* themePackage);

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs();
    void updateTitle();

protected:
    virtual void beginScene()
    {
    }
    virtual void endScene()
    {
    }
    virtual void renderScene();

    void onTreeItemSelected(wxTreeEvent& event);
    void onBrowse(wxCommandEvent& event);
    void onBrowseLayout(wxCommandEvent& event);
    void onListSelected(wxCommandEvent& event);
    void onCanvasRectSize(wxSizeEvent& event);
    void onCanvasGUISize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);

    virtual bool Show(bool show = true);

    void updateUVRC(UVEditorState* uvrc);

private:
    wxTreeCtrl* treeCtrl;
    wxButton* btnBrowse;
    wxTextCtrl* txtUVRCUrl;
    wxListBox* listBox;
    wxButton* btnBrowseLayout;
    wxTextCtrl* txtUVRCUrlLayout;

    ThemeMIMEHandler* mime;
    wxHareCanvas* canvasRect;
    wxHareCanvas* canvasGUI;
    ThemePackage::Ptr themes;
    
    TreeItemBase* currItem;

    bool isModified;

    PreviewSceneListener previewSceneListener;

private:
    DECLARE_ABSTRACT_CLASS(ThemeEditorPage)
    DECLARE_EVENT_TABLE()
};

class ThemeMIMEHandler : public MIMEHandlerPlugin
{
public:
    ThemeMIMEHandler();

    bool newPage(ThemePackage* themes, bool isModified = true);

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);
};

#endif