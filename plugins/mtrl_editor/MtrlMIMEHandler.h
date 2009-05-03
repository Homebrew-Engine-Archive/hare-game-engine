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

class MtrlEditorPage : public EditorPage, public SceneListenerBase
{
public:
    MtrlEditorPage(wxWindow* parent, MtrlMIMEHandler* handler, Material* mtrl);
    virtual ~MtrlEditorPage(); 

    bool isOk() const
    {
        return mtrlStates.size() > 0;
    }

    void addMaterial(Material* mtrl);
    void addMaterialFromFile(const String& url);

    virtual void beginScene();
    virtual void endScene();
    virtual void renderScene();

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

private:
    void onSize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onMouseMove(wxMouseEvent& event);
    void onMouseLeftDown(wxMouseEvent& event);
    void onMouseLeftUp(wxMouseEvent& event);

    virtual bool Show(bool show = true);

    void drawMaterial(Material* mtrl, uint32 color, const PointF& pos);
    void selectMaterial(Material* mtrl);
    Material* subMtrlHitTest(Material* parent, RectF rect, const PointF& mousePos);
    bool replaceSubMtrl(Material* parent, RectF rect, const PointF& mousePos);

    DECLARE_ABSTRACT_CLASS(MtrlEditorPage)
};

class MtrlMIMEHandler : public MIMEHandlerPlugin
{
public:
    MtrlMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

private:
    friend class MtrlEditorPage;
    MtrlEditorPage* page;
};

#endif