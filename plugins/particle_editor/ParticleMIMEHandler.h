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

class ParticleMIMEHandler;

class ParticleEditorPage : public EditorPage, public SceneListenerBase
{
    friend class ParticleMIMEHandler;

private:
    ParticleEditorPage(wxWindow* parent, ParticleMIMEHandler* handler, Particle* par);
    virtual ~ParticleEditorPage(); 

    bool changeParticle(Particle* par);

    bool isOk() const
    {
        return parPtr.pointer() != NULL;
    }

    virtual void beginScene()
    {
    }

    virtual void endScene()
    {
    }
    
    virtual void renderScene()
    {
        if (parPtr)
            parPtr->render();
    }

private:
    wxHareCanvas* canvasParticle;
    SceneManager* sceneParticle;
    Particle::Ptr parPtr;

    ParticleMIMEHandler* mime;

private:
    void onSize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onMouseMove(wxMouseEvent& event);

    DECLARE_ABSTRACT_CLASS(ParticleEditorPage)
};

class ParticleMIMEHandler : public MIMEHandlerPlugin
{
public:
    ParticleMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

private:

    friend class ParticleEditorPage;

    ParticleEditorPage* page;
};

#endif