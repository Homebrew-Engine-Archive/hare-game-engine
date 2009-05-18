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

MtrlEditorPage::MtrlEditorPage(wxWindow* parent, MtrlMIMEHandler* handler)
    : EditorPage(parent), mime(handler), selectedMtrl(0), canDragMtrl(false), isModified(false)
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
    canvas->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MtrlEditorPage::onKeyDown), NULL, this);
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(canvas, 1, wxEXPAND, 0);
    SetSizer(sizer);
    Layout();

    font = (Font*)Object::importObject("/editor/default.font");
    getCanvas()->setFont(font);

    updateTitle();
}

MtrlEditorPage::~MtrlEditorPage()
{
    if (!Manager::isAppShuttingDown())
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }

    mime->page = NULL;
}

bool MtrlEditorPage::saveAs(Material* mtrl)
{
    if (!mtrl)
        return false;

    bool ret = false;

    wxString title = wxString::Format(wxT("Save material [%s] as"), 
        wxString::FromUTF8(mtrl->getClassInfo()->className).c_str());

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        title,
        _T(""),
        _T(""),
        _T("Material (*.material)|*.material|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK)
    {
        FileSystem::getSingletonPtr()->remove("/editor/~material.temp");
        mtrl->saveToXml("/editor/~material.temp");
        String dir = FileSystem::getSingletonPtr()->getRealDir("/editor/~material.temp");
        dir += "/editor/~material.temp";
        wxString tempFile = wxString::FromUTF8(dir.c_str());
        wxFileName fName(tempFile);
        if (fName.FileExists())
        {
            wxString fileName = dlg->GetPath();
            ret = wxCopyFile(fName.GetFullPath(), fileName);
        }
    }
    
    dlg->Destroy();
    return ret;
}

bool MtrlEditorPage::save()
{
    MaterialEditState::List::iterator it = mtrlStates.begin();
    bool allSaved = true;
    for (; it != mtrlStates.end(); ++it)
    {
        MaterialEditState* st = *it;
        if (st->mtrl)
        {
            String url = st->mtrl->getUrl();
            if (!url.empty())
            {
                if (!st->mtrl->saveToXml(url))
                    allSaved = allSaved && saveAs(st->mtrl);
            }
            else
            {
                allSaved = allSaved && saveAs(st->mtrl);
            }
        }
    }
    if (allSaved)
        setModified(false);

    return true;
}

bool MtrlEditorPage::Show(bool show)
{
    if (show)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        if (selectedMtrl)
            Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("MaterialProperity"), selectedMtrl, this);
    }

    return EditorPage::Show(show);
}

void MtrlEditorPage::addMaterial(Material* mtrl, bool isMtrlModified)
{
    if (!mtrl)
        return;
   
    MaterialEditState* newItem = new MaterialEditState;
    newItem->mtrl = mtrl;
    newItem->pos = PointF(100, 100);
    mtrlStates.push_back(newItem);

    if (isMtrlModified)
        setModified(true);
}

void MtrlEditorPage::updateTitle()
{
    if (isModified)
        setTitle(wxT("*[MaterialEditor]"));
    else
        setTitle(wxT("[MaterialEditor]"));
}

void MtrlEditorPage::setModified(bool modified)
{
    if (modified != isModified)
    {
        isModified = modified;
        updateTitle();
    }
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

        drawMaterial(st->mtrl, st->mtrl->getClassInfo(), selectedColor, st->pos, true);
    }
}

void MtrlEditorPage::drawMaterial(Material* mtrl, ClassInfo* cls, uint32 color, const PointF& pos, bool isRoot)
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
    getCanvas()->drawText(rect.minX + 5, rect.minY + 15, 
        mtrl ? mtrl->getClassInfo()->className : cls->className);
    if (mtrl && !mtrl->getUrl().empty())
    {
        String text = isRoot ? "Url:" : "Ref:";
        getCanvas()->drawText(rect.minX + 5, rect.maxY - 5, text + mtrl->getUrl());
    }
    getCanvas()->setColor(oldclr);

    rect.deflate(20, 20, 20, 20);
    getCanvas()->drawImage(rect, mtrl);

    if (!mtrl)
        return;

    if (!isRoot && !mtrl->getUrl().empty())
    {
        return;
    }

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
                Material* subMtrl = (Material*)(*(Object**)attr->data);
                drawMaterial(subMtrl, attr->classInfo, color, subPos, false);
            }
        }
    }
}

void MtrlEditorPage::selectMaterial(Material* mtrl, bool bindObject)
{
    selectedMtrl = mtrl;

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    if (bindObject && selectedMtrl)
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("MaterialProperity"), 
            selectedMtrl, this);
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
            setModified(true);
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

                // Can not edit refed object !!!
                //if (subMtrl && !subMtrl->getUrl().empty())
                //    return NULL;

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
            bool bindObject = subMtrl->getUrl().empty();
            selectMaterial(subMtrl, bindObject);
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

bool MtrlEditorPage::deleteSubMtrl(Material* parent)
{
    if (!parent || !selectedMtrl)
        return false;

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
                if (subMtrl == selectedMtrl)
                {
                    if (subMtrl) 
                        subMtrl->decRef();
                    selectedMtrl = 0;
                    Object** pMtrl = (Object**)attr->data;
                    *pMtrl = 0;
                    setModified(true);
                    return true;
                }
                deleteSubMtrl(subMtrl);
            }
        }
    }
    return false;
}

void MtrlEditorPage::onKeyDown(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_DELETE)
    {
        MaterialEditState::List::iterator it = mtrlStates.begin();
        for (; it != mtrlStates.end(); ++it)
        {
            MaterialEditState* st = *it;
            if (st->mtrl == selectedMtrl)
            {
                mtrlStates.erase(it);
                return;
            }
        }

        if (editMtrl && editMtrl->mtrl)
        {
            deleteSubMtrl(editMtrl->mtrl);
        }
    }
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

bool MtrlMIMEHandler::addMaterial(Material* mtrl, bool isMtrlModified)
{
    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    if (!page)
    {
        epm->getNotebook()->Freeze();
        page = new MtrlEditorPage(epm->getNotebook(), this);
        epm->addEditorPage(page);
        epm->getNotebook()->Thaw();
    }

    page->addMaterial(mtrl, isMtrlModified);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return page->isOk();
}

bool MtrlMIMEHandler::openFile(const wxString& filename)
{
    Material* mtrl = NULL;
    bool isMtrlModified = false;
    
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
        isMtrlModified = true;
    }

    if (!mtrl)
        return false;

    return addMaterial(mtrl, isMtrlModified);
}
