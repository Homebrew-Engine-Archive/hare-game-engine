//***************************************************************
//  File:    AnimationSpritePage.cpp
//  Date:    5/11/2009
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
#include "AnimationSpritePage.h"
#include <wx/xrc/xmlres.h>
#include <wx/splitter.h>

enum TreeMenu
{
    TM_Add,
    TM_Delete,
};

class AnimFrameTreeItemData : public wxTreeItemData
{
public:
    AnimFrame::Ptr frame;
};

IMPLEMENT_ABSTRACT_CLASS(AnimationSpritePage, EditorPage)

AnimationSpritePage::AnimationSpritePage(wxWindow* parent, SpriteMIMEHandler* handler)
    :EditorPage()
    ,mime(handler)
    ,isModified(false)
{
    wxXmlResource::Get()->LoadPanel(this, parent, wxT("idAnimationPanel"));

    treeCtrl = XRCCTRL(*this, "idAnimationTree", wxTreeCtrl);
    animationReview = XRCCTRL(*this, "idAnimationReview", wxPanel);
    frameReview = XRCCTRL(*this, "idFrameReview", wxPanel);
    wxSplitterWindow* splitter = XRCCTRL(*this, "idSplitterH", wxSplitterWindow);
    splitter->SetSashPosition(parent->GetSize().GetHeight() / 2);
    btnPlay = XRCCTRL(*this, "idPlay", wxButton);
    btnStop = XRCCTRL(*this, "idStop", wxButton);
    btnLoop = XRCCTRL(*this, "idLoop", wxToggleButton);



    wxImageList *images = new wxImageList(16, 16, true);
    treeCtrl->SetImageList(images);

    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
    wxBitmap png;
    png.LoadFile(fullPath + wxT("file.png"), wxBITMAP_TYPE_PNG);

    if (png.GetWidth() == 16){
        images->Add(png);
    }else{
        images->Add(wxBitmap(png.ConvertToImage().Rescale(16, 16)));
    }

    treeCtrl->AddRoot(wxT("AnimationSprite"), 0, 0);

    animationCanvas = new wxHareCanvas(animationReview);
    animationReview->GetSizer()->Prepend(animationCanvas, 1, wxEXPAND|wxALL, 0);
    animationScene = getHareApp()->createSceneManager();
    animationScene->setSceneListener(this);
    animationCanvas->getRenderWindow()->setSceneManager(animationScene);

    frameCanvas = new wxHareCanvas(frameReview);
    frameReview->GetSizer()->Add(frameCanvas, 1, wxEXPAND|wxALL, 0);
    frameScene = getHareApp()->createSceneManager();
    frameScene->setSceneListener(this);
    frameCanvas->getRenderWindow()->setSceneManager(frameScene);

    treeCtrl->Connect(wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler(AnimationSpritePage::onBeginDrag), 0, this);
    treeCtrl->Connect(wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler(AnimationSpritePage::onEndDrag), 0, this);
    treeCtrl->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(AnimationSpritePage::onSelChanged), 0, this);
    treeCtrl->Connect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler(AnimationSpritePage::onItemRClick), 0, this);

    treeCtrl->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(AnimationSpritePage::onRMouseDown), 0, this);


    animationCanvas->Connect(wxEVT_SIZE, wxSizeEventHandler(AnimationSpritePage::onAnimationSize), 0, this);
    animationCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(AnimationSpritePage::onAnimationLButtonDown), 0, this);
    animationCanvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(AnimationSpritePage::onAnimationLButtonUp), 0, this);
    animationCanvas->Connect(wxEVT_MOTION, wxMouseEventHandler(AnimationSpritePage::onAnimationMouseMove), 0, this);
    animationReview->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(AnimationSpritePage::onEraseBackground), 0, this);

    frameCanvas->Connect(wxEVT_SIZE, wxSizeEventHandler(AnimationSpritePage::onFrameSize), 0, this);
    frameCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(AnimationSpritePage::onFrameLButtonDown), 0, this);
    frameCanvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(AnimationSpritePage::onFrameLButtonUp), 0, this);
    frameCanvas->Connect(wxEVT_MOTION, wxMouseEventHandler(AnimationSpritePage::onFrameMouseMove), 0, this);
    frameReview->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(AnimationSpritePage::onEraseBackground), 0, this);

    btnPlay->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationSpritePage::onClickPlay), 0, this);
    btnStop->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationSpritePage::onClickStop), 0, this);
    btnLoop->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(AnimationSpritePage::onClickLoop), 0, this);

    Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AnimationSpritePage::onMenuSelected), 0, this);
    Layout();
}

AnimationSpritePage::~AnimationSpritePage()
{
    wxImageList *images = treeCtrl->GetImageList();
    treeCtrl->SetImageList(NULL);
    delete images;

    if (!Manager::isAppShuttingDown()){
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }
}

void AnimationSpritePage::setModified(bool modified)
{
    if (modified != isModified){
        isModified = modified;
        updateTitle();
    }
}

bool AnimationSpritePage::save()
{
    if (!animationSprite)
        return true;

    bool ret = false;
    if (animationSprite->getUrl().empty())
        ret = saveAs();
    else
        ret = animationSprite->saveToXml(animationSprite->getUrl());

    if (ret)
        setModified(false);

    return ret;
}

bool AnimationSpritePage::saveAs()
{
    bool ret = false;

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        _T("Save AnimationSprite resource as"),
        _T(""),
        _T(""),
        _T("AnimationSprite Resource (*.sprite)|*.sprite|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK){
        static const char* tempFileName = "/editor/~AnimationSprite.temp";
        FileSystem::getSingletonPtr()->remove(tempFileName);
        animationSprite->saveToXml(tempFileName);
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

void AnimationSpritePage::beginScene()
{
    if (curSelFrame)
        curSelFrame->getSprite()->moveTo(0, 0);
}

void AnimationSpritePage::endScene()
{
    if (!animationSprite || !btnPlay)
        return;

    if (animationSprite->isStop())
        btnPlay->SetLabel(wxT(">"));
}

void AnimationSpritePage::renderScene()
{
    if (curSelFrame)
        curSelFrame->getSprite()->moveTo(0, 0);

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
}

void AnimationSpritePage::setAnimationSprite(AnimationSprite* sprite)
{
    animationSprite = sprite;
    animationSprite->stop();
    animationSprite->moveTo(0,0);
    animationSprite->setOrigoPos(0,0);
    animationScene->addSprite(animationSprite);
    for (uint32 i = 0; i < animationSprite->getFrameCount(); ++i){
        addAnimationFrame(animationSprite->getFrame(i));
    }
    updateTitle();
}

void AnimationSpritePage::addAnimationFrame(AnimFrame* frame)
{
    if (!animationSprite)
        return;

    AnimFrameTreeItemData* data = new AnimFrameTreeItemData;
    data->frame = frame;
    treeCtrl->AppendItem(treeCtrl->GetRootItem(), wxString::FromUTF8(frame->getSprite()->getClassInfo()->className), 0, 0, data);
    treeCtrl->ExpandAll();
}

void AnimationSpritePage::setCurSelFrame(AnimFrame* frame)
{
    if (!animationSprite)
        return;
    
    frameScene->removeAllSprite();
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();       

    curSelFrame = frame;
    if (frame){
        frameScene->addSprite(frame->getSprite());
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("AnimFrameProperity"), curSelFrame);
    }
}

void AnimationSpritePage::updateTitle()
{
    String title = "[AnimationSpriteEditor]";
    if (animationSprite) title += animationSprite->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}

void AnimationSpritePage::onBeginDrag(wxTreeEvent& event)
{
    if (event.GetItem() != treeCtrl->GetRootItem()){
        dragTreeItem = event.GetItem();

        event.Allow();
    }
}

void AnimationSpritePage::onEndDrag(wxTreeEvent& event)
{
    wxTreeItemId desItem = event.GetItem();
    wxTreeItemId srcItem = dragTreeItem;
    if (desItem.IsOk() && desItem != treeCtrl->GetRootItem() && desItem != srcItem){
        AnimFrameTreeItemData* desData = (AnimFrameTreeItemData*)treeCtrl->GetItemData(desItem);
        AnimFrameTreeItemData* srcData = (AnimFrameTreeItemData*)treeCtrl->GetItemData(srcItem);
       
        wxString desText = treeCtrl->GetItemText(desItem);
        wxString srcText = treeCtrl->GetItemText(srcItem);
        
        int frameSrcID = animationSprite->getFrameID(srcData->frame);
        int frameDesID = animationSprite->getFrameID(desData->frame);

        if (frameSrcID == -1 || frameDesID == -1){
            return;
        }

        treeCtrl->SetItemData(desItem, srcData);
        treeCtrl->SetItemData(srcItem, desData);

        treeCtrl->SetItemText(desItem, srcText);
        treeCtrl->SetItemText(srcItem, desText);

        treeCtrl->SelectItem(desItem);

        animationSprite->swapFrame(frameSrcID, frameDesID);

    }
}

void AnimationSpritePage::onSelChanged(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    if (itemId == treeCtrl->GetRootItem()){
        setCurSelFrame(NULL);
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("AnimationSpriteProperity"), animationSprite);
    }else{
        AnimFrameTreeItemData* data = (AnimFrameTreeItemData*)treeCtrl->GetItemData(itemId);
        setCurSelFrame(data->frame);
    }

}

void AnimationSpritePage::onMenuSelected(wxCommandEvent& event)
{
    switch (event.GetId())
    {
    case TM_Add:
        {
            AnimFrame::Ptr frame = new AnimFrame;
            wxSize size(400, 400);

            FileSystemDialog dlg(this, true, _("Select Sprite File"), wxT("/"), wxDEFAULT_DIALOG_STYLE,
                wxDefaultPosition, size);

            dlg.SetFilter(wxT(".sprite"));

            if (dlg.ShowModal() == wxID_OK){
                String objectUrl = dlg.GetPath().ToUTF8().data();

                Sprite::Ptr imported = (Sprite*)Object::importObject(objectUrl);
                imported = (Sprite*)Object::cloneObject(imported);

                if (imported && imported->isA(&Sprite::CLASS_INFO)){
                    frame->setSprite(imported);
                    animationSprite->addFrame(frame);
                    addAnimationFrame(frame);
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

            AnimFrameTreeItemData* data = (AnimFrameTreeItemData*)treeCtrl->GetItemData(rightMouseHitItem);
            
            animationSprite->removeFrame(data->frame);       
            
            if (data->frame == curSelFrame){
                setCurSelFrame(NULL);
                Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
                Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("AnimationSpriteProperity"), animationSprite);
                treeCtrl->SelectItem(treeCtrl->GetRootItem());
               
            }

            treeCtrl->Delete(rightMouseHitItem);
        }
        break;
    default:
        break;
    }
    
}

void AnimationSpritePage::onItemRClick(wxTreeEvent& event)
{
    rightMouseHitItem = event.GetItem();

    if (rightMouseHitItem == treeCtrl->GetRootItem()){
        wxMenu menu(wxT("menu for AnimationSprite"));
        menu.Append(TM_Add, wxT("add a frame"));
        PopupMenu(&menu);
    }else{
        wxMenu menu(wxT("menu for AnimationSprite"));
        menu.Append(TM_Delete, wxT("delete a frame"));
        PopupMenu(&menu);
    }
    event.Skip();
}

void AnimationSpritePage::onRMouseDown(wxMouseEvent& event)
{
    wxTreeItemId id = treeCtrl->HitTest(event.GetPosition());
    if (id){
        event.Skip();
        return;
    }
        
    wxMenu menu(wxT("menu for AnimationSprite"));
    menu.Append(TM_Add, wxT("add a frame"));
    PopupMenu(&menu);

}

void AnimationSpritePage::onAnimationSize(wxSizeEvent& event)
{
    wxSize size = animationCanvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        animationCanvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        animationCanvas->getRenderWindow()->moveCameraTo(-size.GetWidth()/2, -size.GetHeight()/2);
    }
    event.Skip();
}

void AnimationSpritePage::onAnimationLButtonDown(wxMouseEvent& event)
{
    animationCanvas->CaptureMouse();
    if (!animationSprite || animationSprite->getFrameCount() == 0)
        return;

    animationMousePos = event.GetPosition();

    setCurSelFrame(NULL);
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("AnimationSpriteProperity"), animationSprite);
    treeCtrl->SelectItem(treeCtrl->GetRootItem());
}

void AnimationSpritePage::onAnimationLButtonUp(wxMouseEvent& event)
{
    if (animationCanvas->HasCapture())
        animationCanvas->ReleaseMouse();

    if (!animationSprite || animationSprite->getFrameCount() == 0)
        return;

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("AnimationSpriteProperity"), animationSprite);
}

void AnimationSpritePage::onAnimationMouseMove(wxMouseEvent& event)
{
    if (!animationSprite || animationSprite->getFrameCount() == 0)
        return;

    if (event.Dragging() && event.LeftIsDown()){
        wxPoint offset = event.GetPosition() - animationMousePos;
        animationMousePos = event.GetPosition();
        animationSprite->moveOrigoPos(offset.x, offset.y);
        setModified(true);
    }
}

void AnimationSpritePage::onFrameSize(wxSizeEvent& event)
{
    wxSize size = frameCanvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        frameCanvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        frameCanvas->getRenderWindow()->moveCameraTo(-size.GetWidth()/2, -size.GetHeight()/2);
    }
    event.Skip();
}

void AnimationSpritePage::onFrameLButtonDown(wxMouseEvent& event)
{
    frameCanvas->CaptureMouse();
    if (!curSelFrame)
        return;

    frameMousePos = event.GetPosition(); 

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("AnimFrameProperity"), curSelFrame);
}

void AnimationSpritePage::onFrameLButtonUp(wxMouseEvent& event)
{
    if (frameCanvas->HasCapture())
        frameCanvas->ReleaseMouse();
    
    if (!curSelFrame)
        return;

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("AnimFrameProperity"), curSelFrame);
}

void AnimationSpritePage::onFrameMouseMove(wxMouseEvent& event)
{
    if (!curSelFrame)
        return;

    if (event.Dragging() && event.LeftIsDown()){
        wxPoint offset = event.GetPosition() - frameMousePos;
        frameMousePos = event.GetPosition();
        curSelFrame->getSprite()->moveOrigoPos(offset.x, offset.y);
        setModified(true);
    }
}

void AnimationSpritePage::onEraseBackground(wxEraseEvent& event)
{
}

void AnimationSpritePage::onClickPlay(wxCommandEvent& event)
{
    if (!animationSprite)
        return;

    if (animationSprite->isStop()){
        if (btnLoop->GetValue()){
            animationSprite->play();
        }else{
            animationSprite->playAction();
        }
        btnPlay->SetLabel(wxT("¡¬"));
    }else{
        if (animationSprite->isPause()){
            animationSprite->resume();
            btnPlay->SetLabel(wxT("¡¬"));
        }else{
            animationSprite->pause();
            btnPlay->SetLabel(wxT(">"));
        }
    }

}

void AnimationSpritePage::onClickStop(wxCommandEvent& event)
{
    if (!animationSprite)
        return;

    animationSprite->stop();
    btnPlay->SetLabel(wxT(">"));
}

void AnimationSpritePage::onClickLoop(wxCommandEvent& event)
{
    if (!animationSprite)
        return;

    if (!animationSprite->isStop()){
        if (btnLoop->GetValue()){
            animationSprite->play();
        }else{
            animationSprite->playAction();
        }
        btnPlay->SetLabel(wxT("¡¬"));
    }  
}