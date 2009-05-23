//***************************************************************
//  File:    ParticleMIMEHandler.h
//  Data:    03/20/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef PARTICLEMIMEHANDLER_H
#define PARTICLEMIMEHANDLER_H

#include <wx/harecanvas.h>

class SpriteMIMEHandler;

class ParticleEditorPage : public EditorPage
{
public:
    ParticleEditorPage(wxWindow* parent, SpriteMIMEHandler* handler, ParticleSprite* par);
    virtual ~ParticleEditorPage(); 

    bool setParticle(ParticleSprite* par);

    bool isOk() const
    {
        return parPtr.pointer() != NULL;
    }

    friend class SceneListenerEditorWrapper<ParticleEditorPage>;
    void renderScene();

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs();
    void updateTitle();

private:
    wxHareCanvas* canvasParticle;
    SceneManager* sceneParticle;
    ParticleSprite::Ptr parPtr;
    Font::Ptr font;

    SpriteMIMEHandler* mime;
    
    bool isModified;

private:
    void onSize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onMouseMove(wxMouseEvent& event);

    virtual bool Show(bool show = true);

    DECLARE_ABSTRACT_CLASS(ParticleEditorPage)
};

#endif