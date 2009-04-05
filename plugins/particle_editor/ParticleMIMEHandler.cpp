//***************************************************************
//  File:    ParticleMIMEHandler.cpp
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
#include "PCH.h"
#include "ParticleMIMEHandler.h"
#include <wx/splitter.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

int idParticleWindow = wxNewId();

IMPLEMENT_ABSTRACT_CLASS(ParticleEditorPage, EditorPage)

ParticleEditorPage::ParticleEditorPage(wxWindow* parent, ParticleMIMEHandler* handler, Particle* par)
 : EditorPage(parent), mime(handler)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    canvasParticle = new wxHareCanvas(this, idParticleWindow);
    canvasParticle->Connect(wxEVT_SIZE, wxSizeEventHandler(ParticleEditorPage::onSize), NULL, this);

    sizer->Add(canvasParticle, 1, wxEXPAND, 5);

    SetSizer(sizer);
    Layout();

    sceneParticle = getHareApp()->createSceneManager();
    sceneParticle->setSceneListener(&parListener);
    canvasParticle->getRenderWindow()->setSceneManager(sceneParticle);

    changeParticle(par);

    Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ParticleEditorPage::onEraseBackground), NULL, this);
    canvasParticle->Connect(wxEVT_MOTION, wxMouseEventHandler(ParticleEditorPage::onMouseMove), NULL, this);
}

ParticleEditorPage::~ParticleEditorPage()
{
    if (parPtr)
    {
        parPtr->saveToXml(parPtr->getUrl());
        parPtr = 0;

        if (!Manager::isAppShuttingDown())
        {
            Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        }
    }

    mime->page = NULL;
}

void ParticleEditorPage::onSize(wxSizeEvent& event)
{
    wxSize size = canvasParticle->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0)
    {
        canvasParticle->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        if (parPtr)
            parPtr->moveTo(size.GetWidth() / 2, size.GetHeight() / 2);
    }
}

void ParticleEditorPage::onMouseMove(wxMouseEvent& event)
{
    if (event.Dragging())
    {
        if (parPtr)
        {
            int x, y;
            event.GetPosition(&x, &y);
            parPtr->moveTo((f32)x, (f32)y);
        }
    }
}

void ParticleEditorPage::onEraseBackground(wxEraseEvent& event)
{
}

bool ParticleEditorPage::changeParticle(Particle* par)
{
    if (par == parPtr)
        return true;

    if (parPtr)
    {
        parPtr->saveToXml(parPtr->getUrl());
    }

    parPtr = par;

    if (parPtr)
    {
        parListener.par = parPtr;
        int w, h;
        canvasParticle->GetClientSize(&w, &h);
        parListener.par->fireAt(w / 2, h / 2);
    }
    
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ParticleProperity"), parPtr);

    setTitle(wxT("[ParticleEditor]") + wxString::FromUTF8(parPtr->getUrl().c_str()));

    return true;
}

ParticleMIMEHandler::ParticleMIMEHandler() : page(0)
{
}

bool ParticleMIMEHandler::canHandle(const wxString& filename) const
{
    return filename.Lower().EndsWith(wxT(".particle"));
}

bool ParticleMIMEHandler::openFile(const wxString& filename)
{
    Particle::Ptr par = (Particle*)Object::importObject(filename.ToUTF8().data());

    if (!par)
        return false;

    if (!page)
    {
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Freeze();
        page = new ParticleEditorPage(Manager::getInstancePtr()->getEditorPageManager()->getNotebook(), this, par);
        Manager::getInstancePtr()->getEditorPageManager()->addEditorPage(page);
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Thaw();
    }
    else
        page->changeParticle(par);

    int index = Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->GetPageIndex(page);

    if (index != -1)
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->SetSelection(index);

    return page->isOk();
}
