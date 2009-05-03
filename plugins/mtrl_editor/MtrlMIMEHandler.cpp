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
 : EditorPage(parent), mime(handler), selectedMtrl(0), canDragMtrl(false)
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

        uint32 selectedColor = 0xFF808080;
        if (editMtrl && editMtrl->mtrl == st->mtrl)
            selectedColor = 0xFFFFFFFF;

        drawMaterial(st->mtrl, selectedColor, st->pos);
    }
}

void MtrlEditorPage::drawMaterial(Material* mtrl, uint32 color, const PointF& pos)
{
    RectF rect(0, 0, GRID_SIZE, GRID_SIZE);
    rect.moveTo(pos);

    uint32 borderColor = 0xFFFFFFFF;

    if (selectedMtrl && selectedMtrl == mtrl)
        borderColor = 0xFFFFFF00;
    else
        borderColor = color;
    
    uint32 oldclr = getCanvas()->getColor();
    getCanvas()->setColor(borderColor);
    getCanvas()->drawRect(rect.minX, rect.minY, rect.maxX, rect.maxY);
    getCanvas()->drawText(rect.minX + 5, rect.minY + 15, mtrl ? mtrl->getClassInfo()->className : "NullObject");
    getCanvas()->setColor(oldclr);

    rect.deflate(20, 20, 20, 20);
    getCanvas()->drawImage(rect, mtrl);

    if (mtrl)
    {
        AttVisitor v;
        mtrl->accept(v);
        Attribute::List::iterator it = v.attributes.begin();
        for (; it != v.attributes.end(); ++it)
        {
            Attribute* attr = *it;
            if (attr->attrType == Attribute::attrObject && attr->data && attr->classInfo)
            {
                if (attr->classInfo->isDerivedFrom(&Material::CLASS_INFO))
                {
                    PointF subPos = pos;
                    subPos.move(0, GRID_SIZE);
                    Object* obj = *(Object**)attr->data;
                    drawMaterial((Material*)obj, color, subPos);
                }
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
    if (!canDragMtrl)
        return;
    else
        canDragMtrl = false;

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

        if (replaceSubMtrl(st->mtrl, rect, pt))
        {
            mtrlStates.remove(editMtrl);
            editMtrl = st;
            return;
        }
    }
}

bool MtrlEditorPage::replaceSubMtrl(Material* parent, RectF rect, const PointF& mousePos)
{
    if (!parent)
        return false;

    if (editMtrl->mtrl == parent)
        return false;

    AttVisitor v;
    parent->accept(v);
    Attribute::List::iterator it = v.attributes.begin();
    for (int i = 0; it != v.attributes.end(); ++it)
    {
        Attribute* attr = *it;
        if (attr->attrType == Attribute::attrObject && attr->data && attr->classInfo)
        {
            if (editMtrl->mtrl->isA(attr->classInfo))
            {
                Object* obj = *(Object**)attr->data;
                Material* subMtrl = (Material*)obj;
                rect.move(0, GRID_SIZE);
                if (rect.isPointIn(mousePos))
                {
                    Object** pMtrl = (Object**)attr->data;
                    if (editMtrl->mtrl) 
                        editMtrl->mtrl->addRef();
                    if (subMtrl) 
                        subMtrl->decRef();
                    *pMtrl = editMtrl->mtrl;
                    return true;
                }
                else
                {
                    return replaceSubMtrl(subMtrl, rect, mousePos);
                }
            }
        }
    }
    return false;
}


Material* MtrlEditorPage::subMtrlHitTest(Material* parent, RectF rect, const PointF& mousePos)
{
    if (!parent)
        return NULL;

    AttVisitor v;
    parent->accept(v);
    Attribute::List::iterator it = v.attributes.begin();
    for (int i = 0; it != v.attributes.end(); ++it)
    {
        Attribute* attr = *it;
        if (attr->attrType == Attribute::attrObject && attr->data && attr->classInfo)
        {
            if (attr->classInfo->isDerivedFrom(&Material::CLASS_INFO))
            {
                Object* obj = *(Object**)attr->data;
                Material* subMtrl = (Material*)obj;
                rect.move(0, GRID_SIZE);
                if (rect.isPointIn(mousePos))
                {
                    return subMtrl;
                }
                else
                {
                    return subMtrlHitTest(subMtrl, rect, mousePos);
                }
            }
        }
    }
    return NULL;
}


void MtrlEditorPage::onMouseLeftDown(wxMouseEvent& event)
{
    mouseDownPos = event.GetPosition();

    SetFocus();

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
            canDragMtrl = true;
            return;
        }
        
        Material* subMtrl = subMtrlHitTest(st->mtrl, rect, pt);
        if (subMtrl)
        {
            selectMaterial(subMtrl);
            editMtrl = st;
            mtrlStates.erase(it0);
            mtrlStates.push_front(st);
            canDragMtrl = true;
            return;
        }
    }
    canDragMtrl = false;
}

void MtrlEditorPage::onMouseMove(wxMouseEvent& event)
{
    if (canDragMtrl && editMtrl && event.Dragging() && event.LeftIsDown())
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
