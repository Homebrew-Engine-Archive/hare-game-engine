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

class MtrlEditorPage : public EditorPage, public SceneListenerBase
{
public:
    MtrlEditorPage(wxWindow* parent, MtrlMIMEHandler* handler, Material* mtrl);
    virtual ~MtrlEditorPage(); 

    bool isOk() const
    {
        return mtrlPtr.pointer() != NULL;
    }

    void addMaterial(Material* mtrl);

    virtual void beginScene();
    virtual void endScene();
    virtual void renderScene();

private:
    Material::Ptr mtrlPtr;
    MtrlMIMEHandler* mime;
    wxHareCanvas* canvas;
    SceneManager* scene;

private:
    void onSize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onMouseMove(wxMouseEvent& event);

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