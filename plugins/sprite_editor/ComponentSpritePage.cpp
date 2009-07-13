//***************************************************************
//  File:    ComponentSpritePage.cpp
//  Data:    05/17/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#include "PCH.h"
#include "ComponentSpritePage.h"
#include <wx/xrc/xmlres.h>
#include <wx/splitter.h>

enum TreeMenu
{
    TM_Add,
    TM_Delete,
};

class PartSpriteTreeItemData : public wxTreeItemData
{
public:
    Sprite::Ptr part;
};

IMPLEMENT_ABSTRACT_CLASS(ComponentSpritePage, EditorPage)


ComponentSpritePage::ComponentSpritePage(wxWindow* parent, SpriteMIMEHandler* handler)
    :EditorPage()
    ,mime(handler)
    ,isModified(false)
{
    wxXmlResource::Get()->LoadPanel(this, parent, wxT("idComponentPanel"));
    treeCtrl = XRCCTRL(*this, "idComponentTree", wxTreeCtrl);
    componentReview = XRCCTRL(*this, "idComponentReview", wxPanel);
    partSpriteReview = XRCCTRL(*this, "idPartSpriteReview", wxPanel);
    wxSplitterWindow* splitter = XRCCTRL(*this, "idSplitterH", wxSplitterWindow);
    splitter->SetSashPosition(parent->GetSize().GetHeight() / 2);

    wxImageList *images = new wxImageList(16, 16, true);
    treeCtrl->SetImageList(images);

    images->Add(Manager::loadBitmap(Manager::getInstancePtr()->getAppDir() + wxT("/resources.zip#zip:file.png")));

    treeCtrl->AddRoot(wxT("ComponentSprite"), 0, 0);

    SceneListenerEditorWrapper<ComponentSpritePage>* listener = new SceneListenerEditorWrapper<ComponentSpritePage>(this);

    componentCanvas = new wxHareCanvas(componentReview);
    componentReview->GetSizer()->Prepend(componentCanvas, 1, wxEXPAND|wxALL, 0);
    componentScene = getHareApp()->createSceneManager();
    componentScene->setSceneListener(listener);
    componentCanvas->getRenderWindow()->setSceneManager(componentScene);

    partSpriteCanvas = new wxHareCanvas(partSpriteReview);
    partSpriteReview->GetSizer()->Prepend(partSpriteCanvas, 1, wxEXPAND|wxALL, 0);
    partSpriteScene = getHareApp()->createSceneManager();
    partSpriteScene->setSceneListener(listener);
    partSpriteCanvas->getRenderWindow()->setSceneManager(partSpriteScene);


    treeCtrl->Connect(wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler(ComponentSpritePage::onBeginDrag), 0, this);
    treeCtrl->Connect(wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler(ComponentSpritePage::onEndDrag), 0, this);
    treeCtrl->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(ComponentSpritePage::onSelChanged), 0, this);
    treeCtrl->Connect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler(ComponentSpritePage::onItemRClick), 0, this);

    treeCtrl->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(ComponentSpritePage::onRMouseDown), 0, this);


    componentCanvas->Connect(wxEVT_SIZE, wxSizeEventHandler(ComponentSpritePage::onComponentSize), 0, this);
    componentCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ComponentSpritePage::onComponentLButtonDown), 0, this);
    componentCanvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ComponentSpritePage::onComponentLButtonUp), 0, this);
    componentCanvas->Connect(wxEVT_MOTION, wxMouseEventHandler(ComponentSpritePage::onComponentMouseMove), 0, this);
    componentReview->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ComponentSpritePage::onEraseBackground), 0, this);

    partSpriteCanvas->Connect(wxEVT_SIZE, wxSizeEventHandler(ComponentSpritePage::onPartSpriteSize), 0, this);
    partSpriteCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ComponentSpritePage::onPartSpriteLButtonDown), 0, this);
    partSpriteCanvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ComponentSpritePage::onPartSpriteLButtonUp), 0, this);
    partSpriteCanvas->Connect(wxEVT_MOTION, wxMouseEventHandler(ComponentSpritePage::onPartSpriteMouseMove), 0, this);
    partSpriteReview->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ComponentSpritePage::onEraseBackground), 0, this);



    Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ComponentSpritePage::onMenuSelected), 0, this);
    Layout();
}

ComponentSpritePage::~ComponentSpritePage()
{
    wxImageList *images = treeCtrl->GetImageList();
    treeCtrl->SetImageList(NULL);
    delete images;

    if (!Manager::isAppShuttingDown()){
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }
}

void ComponentSpritePage::setModified(bool modified)
{
    if (modified != isModified){
        isModified = modified;
        updateTitle();
    }
}

bool ComponentSpritePage::save()
{
    if (!componentSprite)
        return true;

    bool ret = false;
    if (componentSprite->getUrl().empty())
        ret = saveAs();
    else
        ret = componentSprite->saveToXml(componentSprite->getUrl());

    if (ret)
        setModified(false);

    return ret;
}

bool ComponentSpritePage::saveAs()
{
    bool ret = false;

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        _T("Save ComponentSprite resource as"),
        _T(""),
        _T(""),
        _T("ComponentSprite Resource (*.sprite)|*.sprite|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK){
        static const char* tempFileName = "/editor/~ComponentSprite.temp";
        FileSystem::getSingletonPtr()->remove(tempFileName);
        componentSprite->saveToXml(tempFileName);
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

void ComponentSpritePage::updateTitle()
{
    String title = "[ComponentSpriteEditor]";
    if (componentSprite) title += componentSprite->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}

void ComponentSpritePage::renderScene()
{
    if (curPartSprite)
        curPartSprite->moveTo(0, 0);

    RenderWindow* renderwindow = RenderSystem::getSingletonPtr()->getCurRenderWindow();
    if (!renderwindow)
        return;

    uint32 width = renderwindow->getWidth();
    uint32 height= renderwindow->getHeight();

    uint32 oldColor = getCanvas()->getColor();
    getCanvas()->setColor(0xff00ff00);
    getCanvas()->drawLine(-1000, height/2, 1000, height/2);
    getCanvas()->drawLine(width/2, -1000, width/2, 1000);
    getCanvas()->setColor(oldColor);

    if (curPartSprite)
        curPartSprite->moveTo(0, 0);
}

void ComponentSpritePage::setComponentSprite(ComponentSprite* sprite)
{
    componentSprite = sprite;
    componentSprite->moveTo(0,0);
    componentScene->addSprite(componentSprite);
    for (uint32 i = 0; i < componentSprite->getPartCount(); ++i){
        addPartSprite(componentSprite->getPart(i));
    }
    updateTitle();
}

void ComponentSpritePage::setCurPartSprite(Sprite* sprite)
{
    if (!componentSprite)
        return;

    partSpriteScene->removeAllSprite();
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();       
    
    curPartSprite = sprite;
    if (curPartSprite){
        partSpriteScene->addSprite(curPartSprite);
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("PartSpriteProperity"), curPartSprite);
    }
}

void ComponentSpritePage::addPartSprite(Sprite* sprite)
{
    if (!componentSprite)
        return;

    PartSpriteTreeItemData* data = new PartSpriteTreeItemData;
    data->part = sprite;
    treeCtrl->AppendItem(treeCtrl->GetRootItem(), wxString::FromUTF8(sprite->getClassInfo()->className), 0, 0, data);
    treeCtrl->ExpandAll();
}

void ComponentSpritePage::onBeginDrag(wxTreeEvent& event)
{
    if (event.GetItem() != treeCtrl->GetRootItem()){
        dragTreeItem = event.GetItem();

        event.Allow();
    }
}

void ComponentSpritePage::onEndDrag(wxTreeEvent& event)
{
    wxTreeItemId desItem = event.GetItem();
    wxTreeItemId srcItem = dragTreeItem;
    if (desItem.IsOk() && desItem != treeCtrl->GetRootItem() && desItem != srcItem){
        PartSpriteTreeItemData* desData = (PartSpriteTreeItemData*)treeCtrl->GetItemData(desItem);
        PartSpriteTreeItemData* srcData = (PartSpriteTreeItemData*)treeCtrl->GetItemData(srcItem);

        wxString desText = treeCtrl->GetItemText(desItem);
        wxString srcText = treeCtrl->GetItemText(srcItem);

        int partSrcID = componentSprite->getPartID(srcData->part);
        int partDesID = componentSprite->getPartID(desData->part);

        if (partSrcID == -1 || partDesID == -1){
            return ;
        }

        treeCtrl->SetItemData(desItem, srcData);
        treeCtrl->SetItemData(srcItem, desData);

        treeCtrl->SetItemText(desItem, srcText);
        treeCtrl->SetItemText(srcItem, desText);

        treeCtrl->SelectItem(desItem);

        componentSprite->swapPart(partSrcID, partDesID);
    }

}

void ComponentSpritePage::onSelChanged(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    if (itemId == treeCtrl->GetRootItem()){
        setCurPartSprite(NULL);
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ComponentSpriteProperity"), componentSprite);
    }else{
        PartSpriteTreeItemData* data = (PartSpriteTreeItemData*)treeCtrl->GetItemData(itemId);
        setCurPartSprite(data->part);
    }
}
    
void ComponentSpritePage::onMenuSelected(wxCommandEvent& event)
{
    switch (event.GetId())
    {
    case TM_Add:
        {
            wxSize size(400, 400);

            FileSystemDialog dlg(this, true, _("FileSystem"), wxT("/"), 0,
                wxDefaultPosition, size);

            if (dlg.ShowModal() == wxID_OK){
                String objectUrl = dlg.GetPath().ToUTF8().data();

                Sprite::Ptr imported = (Sprite*)Object::importObject(objectUrl);
                imported = (Sprite*)Object::cloneObject(imported);

                if (imported && imported->isA(&Sprite::CLASS_INFO)){
                    componentSprite->addPart(imported);
                    addPartSprite(imported);
                }
            }
        }
        break;
    case TM_Delete:
        {
            if (!rightMouseHitItem.IsOk())
                break;

            if (rightMouseHitItem == treeCtrl->GetRootItem())
                break;

            PartSpriteTreeItemData* data = (PartSpriteTreeItemData*)treeCtrl->GetItemData(rightMouseHitItem);

            componentSprite->removePart(data->part);       

            if (data->part == curPartSprite){
                setCurPartSprite(NULL);
                Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
                Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ComponentSpriteProperity"), componentSprite);
                treeCtrl->SelectItem(treeCtrl->GetRootItem());

            }

            treeCtrl->Delete(rightMouseHitItem);
        }
        break;
    default:
        break;
    }
}

void ComponentSpritePage::onItemRClick(wxTreeEvent& event)
{
    rightMouseHitItem = event.GetItem();

    if (rightMouseHitItem == treeCtrl->GetRootItem()){
        wxMenu menu(wxT("menu for ComponentSprite"));
        menu.Append(TM_Add, wxT("add a frame"));
        PopupMenu(&menu);
    }else{
        wxMenu menu(wxT("menu for ComponentSprite"));
        menu.Append(TM_Delete, wxT("delete a frame"));
        PopupMenu(&menu);
    }
    event.Skip();
}

void ComponentSpritePage::onRMouseDown(wxMouseEvent& event)
{
    wxTreeItemId id = treeCtrl->HitTest(event.GetPosition());
    if (id){
        event.Skip();
        return;
    }

    wxMenu menu(wxT("menu for ComponentSprite"));
    menu.Append(TM_Add, wxT("add a part"));
    PopupMenu(&menu);
}

void ComponentSpritePage::onComponentSize(wxSizeEvent& event)
{
    wxSize size = componentCanvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        componentCanvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        componentCanvas->getRenderWindow()->moveCameraTo(-size.GetWidth()/2, -size.GetHeight()/2);
    }
    event.Skip();
}

void ComponentSpritePage::onComponentLButtonDown(wxMouseEvent& event)
{
    componentCanvas->CaptureMouse();
    if (!componentSprite)
        return;

    componentMousePos = event.GetPosition();

    setCurPartSprite(NULL);
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ComponentSpriteProperity"), componentSprite);
    treeCtrl->SelectItem(treeCtrl->GetRootItem());
}

void ComponentSpritePage::onComponentLButtonUp(wxMouseEvent& event)
{
    if (componentCanvas->HasCapture())
        componentCanvas->ReleaseMouse();

    if (!componentSprite)
        return;

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("ComponentSpriteProperity"), componentSprite);
}

void ComponentSpritePage::onComponentMouseMove(wxMouseEvent& event)
{
    if (!componentSprite)
        return;

    if (event.Dragging() && event.LeftIsDown()){
        wxPoint offset = event.GetPosition() - componentMousePos;
        componentMousePos = event.GetPosition();
        componentSprite->moveOrigoPos(offset.x, offset.y);
        setModified(true);
    }
}

void ComponentSpritePage::onPartSpriteSize(wxSizeEvent& event)
{
    wxSize size = partSpriteCanvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        partSpriteCanvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        partSpriteCanvas->getRenderWindow()->moveCameraTo(-size.GetWidth()/2, -size.GetHeight()/2);
    }
    event.Skip();
}

void ComponentSpritePage::onPartSpriteLButtonDown(wxMouseEvent& event)
{
    partSpriteCanvas->CaptureMouse();
    if (!curPartSprite)
        return;

    partSpriteMousePos = event.GetPosition(); 

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("PartSpriteProperity"), curPartSprite);

}

void ComponentSpritePage::onPartSpriteLButtonUp(wxMouseEvent& event)
{
    if (partSpriteCanvas->HasCapture())
        partSpriteCanvas->ReleaseMouse();

    if (!curPartSprite)
        return;

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("PartSpriteProperity"), curPartSprite);

}

void ComponentSpritePage::onPartSpriteMouseMove(wxMouseEvent& event)
{
    if (!curPartSprite)
        return;

    if (event.Dragging() && event.LeftIsDown()){
        wxPoint offset = event.GetPosition() - partSpriteMousePos;
        partSpriteMousePos = event.GetPosition();
        curPartSprite->moveOrigoPos(offset.x, offset.y);
        setModified(true);
    }
}

void ComponentSpritePage::onEraseBackground(wxEraseEvent& event)
{

}
