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

const int MtrlEditorPage::GRID_SIZE = 150;

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
    canvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(MtrlEditorPage::onMouseLeftUp), 0, this);
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(canvas, 1, wxEXPAND, 0);
    SetSizer(sizer);
    Layout();

    font = (Font*)Object::importObject("/editor/default.font");
    getCanvas()->setFont(font);

    addMaterial(mtrl);
}

MtrlEditorPage::~MtrlEditorPage()
{
    if (!Manager::isAppShuttingDown())
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }

    mime->page = NULL;
}

bool MtrlEditorPage::Show(bool show)
{
    if (show)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        if (selectedMtrl)
            Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("MaterialProperity"), selectedMtrl);
    }

    return EditorPage::Show(show);
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
    newItem->pos = PointF(100, 100);
    mtrlStates.push_back(newItem);

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
    MaterialEditState::List::reverse_iterator rit = mtrlStates.rbegin();
    for (; rit != mtrlStates.rend(); ++rit)
    {
        MaterialEditState* st = *rit;
        drawMaterial(st->mtrl, st->pos);
    }
}

void MtrlEditorPage::drawMaterial(Material* mtrl, const PointF& pos)
{
    RectF rect(0, 0, GRID_SIZE, GRID_SIZE);
    rect.moveTo(pos);

    uint32 color = 0xFFFFFFFF;

    if (selectedMtrl && selectedMtrl == mtrl)
        color = 0xFFFFFF00;
    
    uint32 oldclr = getCanvas()->getColor();
    getCanvas()->setColor(color);
    getCanvas()->drawRect(rect.minX, rect.minY, rect.maxX, rect.maxY);
    getCanvas()->drawText(rect.minX + 5, rect.minY + 15, mtrl->getClassInfo()->className);
    getCanvas()->setColor(oldclr);

    rect.deflate(20, 20, 20, 20);
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
                subPos.move(0, GRID_SIZE);
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

void MtrlEditorPage::onMouseLeftUp(wxMouseEvent& event)
{
    if (!editMtrl)
        return;

    PointF pt(event.GetPosition().x, event.GetPosition().y);
    RectF rect(0, 0, GRID_SIZE, GRID_SIZE);

    MaterialEditState::List::iterator it0 = mtrlStates.begin();
    for (; it0 != mtrlStates.end(); ++it0)
    {
        MaterialEditState* st = *it0;

        if (st == editMtrl || st->mtrl == editMtrl->mtrl)
            continue;

        rect.moveTo(st->pos);

        AttVisitor v;
        st->mtrl->accept(v);
        Attribute::List::iterator it1 = v.attributes.begin();
        for (; it1 != v.attributes.end(); ++it1)
        {
            Attribute* attr = *it1;
            if (attr->attrType == Attribute::attrObject && attr->data)
            {
                Object* obj = *(Object**)attr->data;
                if (editMtrl->mtrl->isA(attr->classInfo))
                {
                    Material* subMtrl = (Material*)obj;
                    rect.moveTo(st->pos.x, st->pos.y + GRID_SIZE);
                    if (rect.isPointIn(pt))
                    {
                        Object** pMtrl = (Object**)attr->data;
                        subMtrl->decRef();
                        editMtrl->mtrl->addRef();
                        *pMtrl = editMtrl->mtrl;
                        mtrlStates.remove(editMtrl);
                        editMtrl = st;
                        return;
                    }
                }
            }
        }
    }
}

void MtrlEditorPage::onMouseLeftDown(wxMouseEvent& event)
{
    mouseDownPos = event.GetPosition();

    PointF pt(mouseDownPos.x, mouseDownPos.y);
    RectF rect(0.0f, 0.0f, GRID_SIZE, GRID_SIZE);

    MaterialEditState::List::iterator it0 = mtrlStates.begin();
    for (; it0 != mtrlStates.end(); ++it0)
    {
        MaterialEditState* st = *it0;
        rect.moveTo(st->pos);
        if (rect.isPointIn(pt))
        {
            selectMaterial(st->mtrl);
            editMtrl = st;
            mtrlStates.erase(it0);
            mtrlStates.push_front(st);
            return;
        }

        AttVisitor v;
        st->mtrl->accept(v);
        Attribute::List::iterator it1 = v.attributes.begin();
        for (; it1 != v.attributes.end(); ++it1)
        {
            Attribute* attr = *it1;
            if (attr->attrType == Attribute::attrObject && attr->data)
            {
                Object* obj = *(Object**)attr->data;
                if (obj->isA(&Material::CLASS_INFO))
                {
                    Material* subMtrl = (Material*)obj;
                    rect.moveTo(st->pos.x, st->pos.y + GRID_SIZE);
                    if (rect.isPointIn(pt))
                    {
                        selectMaterial(subMtrl);
                        editMtrl = st;
                        mtrlStates.erase(it0);
                        mtrlStates.push_front(st);
                        return;
                    }
                }
            }
        }
    }
}

void MtrlEditorPage::onMouseMove(wxMouseEvent& event)
{
    if (editMtrl && event.Dragging() && event.LeftIsDown())
    {
        wxPoint offset = event.GetPosition() - mouseDownPos;
        mouseDownPos = event.GetPosition();
        MaterialEditState::List::reverse_iterator rit = mtrlStates.rbegin();
        editMtrl->pos.move(offset.x, offset.y);
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
    return filename.Lower().EndsWith(wxT(".material")) ||
        filename.Lower().EndsWith(wxT(".bmp")) ||
        filename.Lower().EndsWith(wxT(".png")) ||
        filename.Lower().EndsWith(wxT(".tga"));

}

bool MtrlMIMEHandler::openFile(const wxString& filename)
{
    Material* mtrl = NULL;

    if (filename.Lower().EndsWith(wxT(".material")))
        mtrl = (Material*)Object::importObject(filename.ToUTF8().data());
    else if (
        filename.Lower().EndsWith(wxT(".bmp")) ||
        filename.Lower().EndsWith(wxT(".png")) ||
        filename.Lower().EndsWith(wxT(".tga")))
    {
        TextureMtrl* texMtrl = new TextureMtrl();
        texMtrl->fileName = filename.ToUTF8().data();
        texMtrl->postLoaded();
        mtrl = texMtrl;
    }

    if (!mtrl)
        return false;

    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    if (!page)
    {
        epm->getNotebook()->Freeze();
        page = new MtrlEditorPage(epm->getNotebook(), this, mtrl);
        epm->addEditorPage(page);
        epm->getNotebook()->Thaw();
    }
    else
        page->addMaterial(mtrl);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return page->isOk();
}
