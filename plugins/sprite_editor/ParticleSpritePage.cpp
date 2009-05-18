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
#include "ParticleSpritePage.h"
#include <wx/splitter.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

int idParticleWindow = wxNewId();

IMPLEMENT_ABSTRACT_CLASS(ParticleEditorPage, EditorPage)

ParticleEditorPage::ParticleEditorPage(wxWindow* parent, SpriteMIMEHandler* handler, ParticleSprite* par)
 : EditorPage(parent), mime(handler)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    canvasParticle = new wxHareCanvas(this, idParticleWindow);
    canvasParticle->Connect(wxEVT_SIZE, wxSizeEventHandler(ParticleEditorPage::onSize), NULL, this);

    sizer->Add(canvasParticle, 1, wxEXPAND, 5);

    SetSizer(sizer);
    Layout();

    sceneParticle = getHareApp()->createSceneManager();
    sceneParticle->setSceneListener(this);
    canvasParticle->getRenderWindow()->setSceneManager(sceneParticle);

    Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ParticleEditorPage::onEraseBackground), NULL, this);
    canvasParticle->Connect(wxEVT_MOTION, wxMouseEventHandler(ParticleEditorPage::onMouseMove), NULL, this);

    font = (Font*)Object::importObject("/editor/default.font");
    getCanvas()->setFont(font);

    setParticle(par);
}

ParticleEditorPage::~ParticleEditorPage()
{
    if (parPtr){
        parPtr = 0;
        if (!Manager::isAppShuttingDown()){
            Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        }
    }
}

void ParticleEditorPage::renderScene()
{
    String fps = StringUtil::format("FPS : %.2f", getTimer().getFPS());
    getCanvas()->drawText(10, 20, fps);
    if (parPtr)
        parPtr->render();
}


bool ParticleEditorPage::Show(bool show)
{
    if (show)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        if (parPtr)
            Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ParticleProperity"), parPtr);
    }
    return EditorPage::Show(show);
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
            parPtr->moveTo((float)x, (float)y);
        }
    }
}

void ParticleEditorPage::onEraseBackground(wxEraseEvent& event)
{
}

bool ParticleEditorPage::setParticle(ParticleSprite* par)
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
        int w, h;
        canvasParticle->GetClientSize(&w, &h);
        parPtr->fireAt(w / 2, h / 2);
    }
    
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ParticleProperity"), parPtr);

    updateTitle();

    return true;
}

void ParticleEditorPage::setModified(bool modified)
{
    if (modified != isModified){
        isModified = modified;
        updateTitle();
    }
}

bool ParticleEditorPage::save()
{
    if (!parPtr)
        return true;

    bool ret = false;
    if (parPtr->getUrl().empty())
        ret = saveAs();
    else
        ret = parPtr->saveToXml(parPtr->getUrl());

    if (ret)
        setModified(false);

    return ret;
}

bool ParticleEditorPage::saveAs()
{
    bool ret = false;

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        _T("Save ParticleSprite resource as"),
        _T(""),
        _T(""),
        _T("ParticleSprite Resource (*.sprite)|*.sprite|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK){
        static const char* tempFileName = "/editor/~ParticleSprite.temp";
        FileSystem::getSingletonPtr()->remove(tempFileName);
        parPtr->saveToXml(tempFileName);
        String dir = FileSystem::getSingletonPtr()->getRealDir(tempFileName);
        dir += tempFileName;
        wxString tempFile = wxString::FromUTF8(dir.c_str());
        wxFileName fName(tempFile);
        if (fName.FileExists()){
            wxString fileName = dlg->GetPath();
            ret = wxCopyFile(fName.GetFullPath(), fileName);
        }
    }

    dlg->Destroy();
    return ret;
}
void ParticleEditorPage::updateTitle()
{
    String title = "[ParticleSpriteEditor]";
    if (parPtr) title += parPtr->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}