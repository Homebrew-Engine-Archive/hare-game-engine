//***************************************************************
//  File:    MtrlMIMEHandler.h
//  Data:    04/18/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef MTRLMIMEHANDLER_H
#define MTRLMIMEHANDLER_H

#include <wx/harecanvas.h>

class MtrlMIMEHandler;

class MaterialEditState : public Object
{
    HARE_DECLARE_DYNAMIC_CLASS(MaterialEditState)
public:
    Material::Ptr mtrl;
    PointF pos;
};

class MtrlEditorPage : public EditorPage
{
public:
    MtrlEditorPage(wxWindow* parent, MtrlMIMEHandler* handler);
    virtual ~MtrlEditorPage(); 

    bool isOk() const
    {
        return mtrlStates.size() > 0;
    }

    void addMaterial(Material* mtrl, bool isMtrlModified);

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs(Material* mtrl);

    friend class SceneListenerEditorWrapper<MtrlEditorPage>;
    void renderScene();

private:
    MaterialEditState::List mtrlStates;
    MaterialEditState::Ptr editMtrl;
    Material::Ptr selectedMtrl;
    Font::Ptr font;

    MtrlMIMEHandler* mime;
    wxHareCanvas* canvas;
    SceneManager* scene;

    static const int GRID_SIZE;

    wxPoint mouseDownPos;
    bool canDragMtrl;
    bool isModified;

private:
    void onSize(wxSizeEvent& event);
    void onKeyDown(wxKeyEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onMouseMove(wxMouseEvent& event);
    void onMouseLeftDown(wxMouseEvent& event);
    void onMouseLeftUp(wxMouseEvent& event);

    virtual bool Show(bool show = true);

    void drawMaterial(Material* mtrl, ClassInfo* cls, uint32 color, const PointF& pos, bool isRoot);
    void selectMaterial(Material* mtrl, bool bindObject = true);
    Material* subMtrlHitTest(Material* parent, RectF rect, const PointF& mousePos);
    bool replaceSubMtrl(Material* parent, RectF rect, const PointF& mousePos);
    bool deleteSubMtrl(Material* parent);

    void updateTitle();

    DECLARE_ABSTRACT_CLASS(MtrlEditorPage)
};

class MtrlMIMEHandler : public MIMEHandlerPlugin
{
public:
    MtrlMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

    bool addMaterial(Material* mtrl, bool isMtrlModified);

private:
    friend class MtrlEditorPage;
    MtrlEditorPage* page;
};

#endif