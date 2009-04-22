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

HARE_IMPLEMENT_DYNAMIC_CLASS(MaterialEditState, Object, 0)
{
}

IMPLEMENT_ABSTRACT_CLASS(MtrlEditorPage, EditorPage)

MtrlEditorPage::MtrlEditorPage(wxWindow* parent, MtrlMIMEHandler* handler, Material* mtrl)
 : EditorPage(parent), mime(handler), selectedMtrl(0)
{
    Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(MtrlEditorPage::onEraseBackground), NULL, this);

    canvas = new wxHareCanvas(this);
    scene = getHareApp()->createSceneManager();
    scene->setSceneListener(this);
    canvas->getRenderWindow()->setSceneManager(scene);
    
    canvas->Connect(wxEVT_SIZE, wxSizeEventHandler(MtrlEditorPage::onSize), 0, this);
    canvas->Connect(wxEVT_MOTION, wxMouseEventHandler(MtrlEditorPage::onMouseMove), 0, this);
    canvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MtrlEditorPage::onMouseLeftDown), 0, this);
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(canvas, 1, wxEXPAND, 0);
    SetSizer(sizer);
    Layout();

    addMaterial(mtrl);
}

MtrlEditorPage::~MtrlEditorPage()
{
    if (!Manager::isAppShuttingDown())
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }

    mime->page = NULL;

    delete canvas;
}

void MtrlEditorPage::addMaterialFromFile(const String& url)
{
    TextureMtrl::Ptr texMtrl = new TextureMtrl();
    texMtrl->fileName = url;
    texMtrl->postLoaded();
    addMaterial(texMtrl);
}

void MtrlEditorPage::addMaterial(Material* mtrl)
{
    if (!mtrl)
        return;
   
    MaterialEditState* newItem = new MaterialEditState;
    newItem->mtrl = mtrl;
    newItem->pos = PointF(200.0f, 200.0f);
    mtrlStates.push_back(newItem);

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("MaterialProperity"), mtrl);

    setTitle(wxT("[MaterialEditor]"));
}

void MtrlEditorPage::beginScene()
{

}

void MtrlEditorPage::endScene()
{

}

void MtrlEditorPage::renderScene()
{
    MaterialEditState::List::iterator it = mtrlStates.begin();
    for (; it != mtrlStates.end(); ++it)
    {
        MaterialEditState* st = *it;
        drawMaterial(st->mtrl, st->pos);
    }
}

void MtrlEditorPage::drawMaterial(Material* mtrl, const PointF& pos)
{
    RectF rect(0.0f, 0.0f, 100.0f, 100.0f);
    rect.moveTo(pos);

    uint32 color = 0xFFFFFFFF;

    if (selectedMtrl && selectedMtrl == mtrl)
        color = 0xFFFFFF00;
    
    uint32 oldclr = getCanvas()->getColor();
    getCanvas()->setColor(color);
    getCanvas()->drawRect(rect.minX, rect.minY, rect.maxX, rect.maxY);
    getCanvas()->setColor(oldclr);

    rect.deflate(5, 5, 5, 5);
    getCanvas()->drawImage(rect, mtrl);

    AttVisitor v;
    mtrl->accept(v);
    Attribute::List::iterator it = v.attributes.begin();
    for (; it != v.attributes.end(); ++it)
    {
        Attribute* attr = *it;
        if (attr->attrType == Attribute::attrObject && attr->data)
        {
            Object* obj = *(Object**)attr->data;
            if (obj->isA(&Material::CLASS_INFO))
            {
                Material* subMtrl = (Material*)obj;
                PointF subPos = pos;
                subPos.move(101, 0);
                drawMaterial(subMtrl, subPos);
            }
        }
    }
}

void MtrlEditorPage::selectMaterial(Material* mtrl)
{
    selectedMtrl = mtrl;

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    if (selectedMtrl)
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("MaterialProperity"), 
            selectedMtrl);
}

void MtrlEditorPage::onSize(wxSizeEvent& event)
{
    wxSize size = canvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0)
        canvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
}

void MtrlEditorPage::onMouseLeftDown(wxMouseEvent& event)
{
    mouseDownPos = event.GetPosition();

    PointF pt(mouseDownPos.x, mouseDownPos.y);
    RectF rect(0.0f, 0.0f, 100.0f, 100.0f);

    MaterialEditState::List::reverse_iterator rit = mtrlStates.rbegin();
    for (; rit != mtrlStates.rend(); ++rit)
    {
        MaterialEditState* st = *rit;
        rect.moveTo(st->pos);
        if (rect.isPointIn(pt))
        {
            selectMaterial(st->mtrl);
            mtrlStates.push_back(st);
            mtrlStates.erase(rit.base());
            return;
        }

        AttVisitor v;
        st->mtrl->accept(v);
        Attribute::List::iterator it = v.attributes.begin();
        for (; it != v.attributes.end(); ++it)
        {
            Attribute* attr = *it;
            if (attr->attrType == Attribute::attrObject && attr->data)
            {
                Object* obj = *(Object**)attr->data;
                if (obj->isA(&Material::CLASS_INFO))
                {
                    Material* subMtrl = (Material*)obj;
                    rect.moveTo(st->pos.x + 101, st->pos.y);
                    if (rect.isPointIn(pt))
                    {
                        selectMaterial(subMtrl);
                        mtrlStates.push_back(st);
                        mtrlStates.erase(rit.base());
                        return;
                    }
                }
            }
        }
    }
}

void MtrlEditorPage::onMouseMove(wxMouseEvent& event)
{
    if (selectedMtrl && event.Dragging() && event.LeftIsDown())
    {
        wxPoint offset = event.GetPosition() - mouseDownPos;
        mouseDownPos = event.GetPosition();
        MaterialEditState::List::reverse_iterator rit = mtrlStates.rbegin();
        for (; rit != mtrlStates.rend(); ++rit)
        {
            MaterialEditState* st = *rit;
            if (st->mtrl == selectedMtrl)
            {
                st->pos.move(offset.x, offset.y);
                return;
            }
        }
    }
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
