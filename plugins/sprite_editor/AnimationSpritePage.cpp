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

    treeCtrl->AddRoot(wxT("AnimationSprite"));

    animationCanvas = new wxHareCanvas(animationReview);
    animationReview->GetSizer()->Add(animationCanvas, 1, wxEXPAND|wxALL, 0);
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
    treeCtrl->Connect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler(AnimationSpritePage::OnItemRClick), 0, this);

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

    Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AnimationSpritePage::onMenuSelected), 0, this);
}

AnimationSpritePage::~AnimationSpritePage()
{

}

void AnimationSpritePage::setModified(bool modified)
{

}

bool AnimationSpritePage::save()
{
    return true;
}

bool AnimationSpritePage::saveAs()
{
    return true;
}

void AnimationSpritePage::beginScene()
{
    if (curSelFrame)
        curSelFrame->getSprite()->moveTo(0, 0);
}

void AnimationSpritePage::endScene()
{

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
    animationSprite->play();
    animationSprite->moveTo(0,0);
    animationSprite->setOrigoPos(0,0);
    animationScene->addSprite(animationSprite);

}

void AnimationSpritePage::addAnimationFrame(AnimFrame* frame)
{
    if (!animationSprite)
        return;

    animationSprite->addFrame(frame);
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

}

void AnimationSpritePage::onBeginDrag(wxTreeEvent& event)
{

}

void AnimationSpritePage::onEndDrag(wxTreeEvent& event)
{

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

            FileSystemDialog dlg(this, true, _("FileSystem"), wxT("/"), 0,
                wxDefaultPosition, size);

            if (dlg.ShowModal() == wxID_OK){
                String objectUrl = dlg.GetPath().ToUTF8().data();

                Sprite::Ptr imported = (Sprite*)Object::importObject(objectUrl);
                imported = (Sprite*)Object::cloneObject(imported);

                if (imported && imported->isA(&Sprite::CLASS_INFO)){
                    frame->setSprite(imported);
                    addAnimationFrame(frame);
                    AnimFrameTreeItemData* data = new AnimFrameTreeItemData;
                    data->frame = frame;
                    treeCtrl->AppendItem(treeCtrl->GetRootItem(), wxString::FromUTF8(imported->CLASS_INFO.getClassName()), -1, -1, data);
                }
            }
        }
        break;
    case TM_Delete:
        {
            if (!rightMouseHitItem.IsOk())
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

void AnimationSpritePage::OnItemRClick(wxTreeEvent& event)
{
    rightMouseHitItem = event.GetItem();

    wxMenu menu(wxT("menu for AnimationSprite"));
    menu.Append(TM_Delete, wxT("delete a frame"));
    PopupMenu(&menu);
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



