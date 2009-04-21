//***************************************************************
//  File:    MtrlMIMEHandler.cpp
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
#include "PCH.h"
#include "MtrlMIMEHandler.h"
#include <wx/panel.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

IMPLEMENT_ABSTRACT_CLASS(MtrlEditorPage, EditorPage)

MtrlEditorPage::MtrlEditorPage(wxWindow* parent, MtrlMIMEHandler* handler, Material* mtrl)
 : EditorPage(parent), mime(handler)
{
    Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(MtrlEditorPage::onEraseBackground), NULL, this);

    canvas = new wxHareCanvas(this);
    scene = getHareApp()->createSceneManager();
    scene->setSceneListener(this);
    canvas->getRenderWindow()->setSceneManager(scene);
    canvas->Connect(wxEVT_SIZE, wxSizeEventHandler(MtrlEditorPage::onSize), 0, this);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(canvas, 1, wxEXPAND, 0);
    SetSizer(sizer);
    Layout();

    addMaterial(mtrl);
}

MtrlEditorPage::~MtrlEditorPage()
{
    if (mtrlPtr)
    {
        if (!Manager::isAppShuttingDown())
        {
            Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        }
    }

    mime->page = NULL;

    delete canvas;
}

void MtrlEditorPage::addMaterial(Material* mtrl)
{
    if (mtrl == mtrlPtr)
        return;

    if (mtrlPtr)
    {

    }

    mtrlPtr = mtrl;

    if (mtrlPtr)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("MaterialProperity"), mtrlPtr);
    }

    setTitle(wxT("[MaterialEditor]") + wxString::FromUTF8(mtrlPtr->getUrl().c_str()));
}

void MtrlEditorPage::beginScene()
{

}

void MtrlEditorPage::endScene()
{

}

void MtrlEditorPage::renderScene()
{
    getCanvas()->drawImage(0, 0, mtrlPtr);
}

void MtrlEditorPage::onSize(wxSizeEvent& event)
{
    wxSize size = canvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0)
        canvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
}

void MtrlEditorPage::onMouseMove(wxMouseEvent& event)
{
}

void MtrlEditorPage::onEraseBackground(wxEraseEvent& event)
{
}

MtrlMIMEHandler::MtrlMIMEHandler() : page(0)
{
}

bool MtrlMIMEHandler::canHandle(const wxString& filename) const
{
    return filename.Lower().EndsWith(wxT(".material"));
}

bool MtrlMIMEHandler::openFile(const wxString& filename)
{
    Material* mtrl = (Material*)Object::importObject(filename.ToUTF8().data());

    if (!mtrl)
        return false;

    if (!page)
    {
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Freeze();
        page = new MtrlEditorPage(Manager::getInstancePtr()->getEditorPageManager()->getNotebook(), 
            this, mtrl);
        Manager::getInstancePtr()->getEditorPageManager()->addEditorPage(page);
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Thaw();
    }
    else
        page->addMaterial(mtrl);

    int index = Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->GetPageIndex(page);

    if (index != -1)
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->SetSelection(index);

    return page->isOk();
}
