//***************************************************************
//  File:    SceneMIMEHandler.cpp
//  Date:    5/19/2009
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
#include "SceneMIMEHandler.h"
#include <wx/xrc/xmlres.h>
#include <wx/splitter.h>
#include "drag_hand.xpm"

class SceneTreeClinetData : public wxTreeItemData
{
public:
    Sprite::Ptr sprite;
};

class SpriteClientData : public wxClientData
{
public:
    Sprite::Ptr sprite;
};

void searchSprite(const String& basePath, wxListBox* listBox)
{
    FileSystem* fs = FileSystem::getSingletonPtr();
    StringVector files = fs->enumFiles(basePath);
    for (size_t i = 0; i < files.size(); ++i){
        String fullFileName = basePath + files[i];
        if (fs->isDir(fullFileName)){
            searchSprite(fullFileName + "/", listBox);
        }else{
            String baseName;
            String ext;
            StringUtil::splitFilename(fullFileName, baseName, ext);
            StringUtil::toLowerCase(ext);
            if (ext == "sprite"){
                Sprite::Ptr obj = (Sprite*)Object::importObject(fullFileName.c_str());
                obj = (Sprite*)Object::cloneObject(obj);
                if (obj){
                    SpriteClientData* data = new SpriteClientData;
                    data->sprite = obj;
                    wxString item = wxString::FromUTF8((basePath + files[i]).c_str());
                    listBox->Insert(item, 0, data);            
                }
            }
        } 
    }
}

IMPLEMENT_ABSTRACT_CLASS(ScenePage, EditorPage)

ScenePage::ScenePage(wxWindow* parent, SceneMIMEHandler* handler, SceneManager* s)
    :mime(handler)
    ,isModified(false)
{
    wxXmlResource::Get()->LoadPanel(this, parent, wxT("idSceneEditPanel"));
    
    sceneTree = XRCCTRL(*this, "idSceneTree", wxTreeCtrl);
    scenePanel = XRCCTRL(*this, "idScenePanel", wxPanel);
    spritePanel = XRCCTRL(*this, "idSpriteView", wxPanel);
    spriteListCtrl = XRCCTRL(*this, "idSpriteList", wxListBox);
    btnAddSprite = XRCCTRL(*this, "idAddSprite", wxToggleButton);;

    sceneTree->AddRoot(wxT("scene"));

    sceneCanvas = new wxHareCanvas(scenePanel);
    scenePanel->GetSizer()->Add(sceneCanvas, 1, wxEXPAND|wxALL, 0);
    sceneManager = s;
    sceneManager->setSceneListener(this);
    sceneCanvas->getRenderWindow()->setSceneManager(sceneManager);

    spriteCanvas = new wxHareCanvas(spritePanel);
    spritePanel->GetSizer()->Add(spriteCanvas, 1, wxEXPAND|wxALL, 0);
    spriteScene  = getHareApp()->createSceneManager();
    spriteCanvas->getRenderWindow()->setSceneManager(spriteScene);

    sceneTree->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(ScenePage::onTreeItemSelected), 0, this);

    sceneCanvas->Connect(wxEVT_SIZE, wxSizeEventHandler(ScenePage::onSceneSize), 0, this);
    sceneCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ScenePage::onSceneLButtonDown), 0, this);
    sceneCanvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ScenePage::onSceneLButtonUp), 0, this);
    sceneCanvas->Connect(wxEVT_MOTION, wxMouseEventHandler(ScenePage::onSceneMouseMove), 0, this);
    scenePanel->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ScenePage::onEraseBackground), 0, this);

    sceneCanvas->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(ScenePage::onSceneRButtonDown), 0, this);

    spriteCanvas->Connect(wxEVT_SIZE, wxSizeEventHandler(ScenePage::onSpriteSceneSize), 0, this);
    spriteListCtrl->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(ScenePage::onListBoxItemSelected), 0, this);

    Layout();
    searchSprite("/", spriteListCtrl);

    font = (Font*)Object::importObject("/editor/default.font");
    dragCursor = new wxCursor((const char**)drag_hand_xpm);
}

ScenePage::~ScenePage()
{
    if (!Manager::isAppShuttingDown()){
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }

    delete dragCursor;
}

void ScenePage::setModified(bool modified)
{
    if (modified != isModified){
        isModified = modified;
        updateTitle();
    }
}

bool ScenePage::save()
{
    if (!sceneManager)
        return true;

    bool ret = false;
    if (sceneManager->getUrl().empty())
        ret = saveAs();
    else
        ret = sceneManager->saveToXml(sceneManager->getUrl());

    if (ret)
        setModified(false);

    return ret;
}

bool ScenePage::saveAs()
{
    bool ret = false;

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        _T("Save Scene resource as"),
        _T(""),
        _T(""),
        _T("Scene Resource (*.scene)|*.scene|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK){
        static const char* tempFileName = "/editor/~Scene.temp";
        FileSystem::getSingletonPtr()->remove(tempFileName);
        sceneManager->saveToXml(tempFileName);
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

void ScenePage::beginScene()
{

}

void ScenePage::endScene()
{

}

void ScenePage::renderScene()
{
    if (curAddSprite)
        curAddSprite->render();

    Font::Ptr f = getCanvas()->getFont();
    getCanvas()->setFont(font);
    PointF pos = sceneCanvas->getRenderWindow()->getCameraPos();
    String str = StringUtil::format("camera position x = %d,y = %d fps:%.2f", (int)pos.x, (int)pos.y, getTimer().getFPS());
    getCanvas()->drawText(0, 20, str);

}

void ScenePage::updateTitle()
{
    String title = "[SceneEditor]";
    if (sceneManager) title += sceneManager->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}

void ScenePage::onTreeItemSelected(wxTreeEvent& event)
{
    if (event.GetItem() == sceneTree->GetRootItem()){
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        sceneCanvas->getRenderWindow()->moveCameraTo(0, 0);
        curEditSprite = NULL;
    }else{
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        SceneTreeClinetData* data = (SceneTreeClinetData*)sceneTree->GetItemData(event.GetItem());
        curEditSprite = data->sprite;
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("SpriteProperity"), curEditSprite);
        int w = sceneCanvas->getRenderWindow()->getWidth();
        int h = sceneCanvas->getRenderWindow()->getHeight();
        sceneCanvas->getRenderWindow()->moveCameraTo(curEditSprite->getPosition().x - w / 2, curEditSprite->getPosition().y - h / 2);

    }
    curAddSprite = NULL;
    btnAddSprite->SetValue(false);
}

void ScenePage::onSceneSize(wxSizeEvent& event)
{
    wxSize size = sceneCanvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        sceneCanvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        sceneCanvas->getRenderWindow()->moveCameraTo(-size.GetWidth()/2, -size.GetHeight()/2);
    }
    event.Skip();
}

void ScenePage::onSceneLButtonDown(wxMouseEvent& event)
{
    if (btnAddSprite->GetValue()){
        Sprite::Ptr sprite = (Sprite*)Object::cloneObject(curAddSprite);
        wxPoint pos = event.GetPosition();
        PointF  cameraPos = sceneCanvas->getRenderWindow()->getCameraPos();
        sprite->moveTo(pos.x + cameraPos.x, pos.y + cameraPos.y);
        sceneManager->addSprite(sprite);
        addSpriteToScene(sprite);
    }else{

    }
}

void ScenePage::onSceneLButtonUp(wxMouseEvent& event)
{
    if (btnAddSprite->GetValue()){

    }else{

    }
}

void ScenePage::onSceneMouseMove(wxMouseEvent& event)
{
    if (event.Dragging()){
        if (event.LeftIsDown()){

        }else if (event.RightIsDown()){
            wxPoint offset = event.GetPosition() - rightDownPos;
            rightDownPos = event.GetPosition();

            sceneCanvas->getRenderWindow()->moveCamera(-offset.x, -offset.y);
            wxSetCursor(*dragCursor);
        }
    }

    if (btnAddSprite->GetValue()){
        if (curAddSprite){
            wxPoint pos = event.GetPosition();
            curAddSprite->moveTo(pos.x, pos.y);
        }
    }else{

    }
}

void ScenePage::onSceneRButtonDown(wxMouseEvent& event)
{
    rightDownPos = event.GetPosition();
    wxSetCursor(*dragCursor);
}

void ScenePage::onSceneRButtonUp(wxMouseEvent& event)
{

}

void ScenePage::onSpriteSceneSize(wxSizeEvent& event)
{
    wxSize size = spriteCanvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        spriteCanvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        spriteCanvas->getRenderWindow()->moveCameraTo(-size.GetWidth()/2, -size.GetHeight()/2);
    }
    event.Skip();
}

void ScenePage::onListBoxItemSelected(wxCommandEvent& event)
{
    SpriteClientData* data = (SpriteClientData*)spriteListCtrl->GetClientObject(event.GetSelection());
    spriteScene->removeAllSprite();
    data->sprite->moveTo(0, 0);
    spriteScene->addSprite(data->sprite);
    curAddSprite = (Sprite*)Object::cloneObject(data->sprite);
    curAddSprite->moveTo(0,0);
}

void ScenePage::addSpriteToScene(Sprite* sprite)
{
    SceneTreeClinetData* data = new SceneTreeClinetData;
    data->sprite = sprite;
    sceneTree->AppendItem(sceneTree->GetRootItem(), wxString::FromUTF8(data->sprite->getClassInfo()->className), -1, -1, data);
}

void ScenePage::onEraseBackground(wxEraseEvent& event)
{
}



SceneMIMEHandler::SceneMIMEHandler()
{

}

bool SceneMIMEHandler::canHandle(const wxString& filename) const
{
    if (!filename.Lower().EndsWith(wxT(".scene")))
        return false;

    Object::Ptr obj = Object::importObject(filename.ToUTF8().data());
    if (!obj)
        return false;

    if (obj->getClassInfo()->isDerivedFrom(&SceneManager::CLASS_INFO))
        return true;

    return false;
}

bool SceneMIMEHandler::openFile(const wxString& filename)
{
    Object::Ptr obj = Object::importObject(filename.ToUTF8().data());

    if (!obj)
        return false;

    if (obj->getClassInfo()->isDerivedFrom(&SceneManager::CLASS_INFO)){
        return newPageScene((SceneManager*)obj.pointer(), false);
    }

    return false;
}

bool SceneMIMEHandler::newPageScene(SceneManager* scene, bool isModified)
{
    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    epm->getNotebook()->Freeze();
    ScenePage* page = new ScenePage(epm->getNotebook(), this, scene);
    epm->addEditorPage(page);
    epm->getNotebook()->Thaw();

    if (isModified)
        page->setModified(isModified);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return true;
}
