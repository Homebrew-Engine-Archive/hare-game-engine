//***************************************************************
//  File:    SpriteMIMEHandler.cpp
//  Date:    5/10/2009
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
#include "SpriteMIMEHandler.h"
#include "ImageSpritePage.h"
#include "AnimationSpritePage.h"

SpriteMIMEHandler::SpriteMIMEHandler()
{
    
}


bool SpriteMIMEHandler::canHandle(const wxString& filename) const
{
    if (!filename.Lower().EndsWith(wxT(".sprite")))
        return false;

    Object::Ptr obj = Object::importObject(filename.ToUTF8().data());

    if (obj->getClassInfo()->isDerivedFrom(&ImageSprite::CLASS_INFO))
        return true;
    if (obj->getClassInfo()->isDerivedFrom(&ComponentSprite::CLASS_INFO))
        return true;
    if (obj->getClassInfo()->isDerivedFrom(&AnimationSprite::CLASS_INFO))
        return true;

    return false;
}

bool SpriteMIMEHandler::openFile(const wxString& filename)
{
    Object::Ptr obj = Object::importObject(filename.ToUTF8().data());

    if (obj->getClassInfo()->isDerivedFrom(&ImageSprite::CLASS_INFO)){
        newPageImageSprite((ImageSprite*)obj.pointer(), false);
        return true;
    }

    if (obj->getClassInfo()->isDerivedFrom(&ComponentSprite::CLASS_INFO)){
        return true;
    }

    if (obj->getClassInfo()->isDerivedFrom(&AnimationSprite::CLASS_INFO)){
        newPageAnimationSprite((AnimationSprite*)obj.pointer(), false);
        return true;
    }

    return false;
}

bool SpriteMIMEHandler::newPageImageSprite(ImageSprite* imageSprite, bool isModified)
{
    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    epm->getNotebook()->Freeze();
    ImageSpritePage* page = new ImageSpritePage(epm->getNotebook(), this);
    epm->addEditorPage(page);
    epm->getNotebook()->Thaw();

    page->setImageSprite(imageSprite);

    if (isModified)
        page->setModified(isModified);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return true;
}

bool SpriteMIMEHandler::newPageAnimationSprite(AnimationSprite* animation, bool isModified)
{
    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    epm->getNotebook()->Freeze();
    AnimationSpritePage* page = new AnimationSpritePage(epm->getNotebook(), this);
    epm->addEditorPage(page);
    epm->getNotebook()->Thaw();

    page->setAnimationSprite(animation);

    if (isModified)
        page->setModified(isModified);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return true;
    

}

bool SpriteMIMEHandler::newPageComponentSprite(ComponentSprite* component, bool isModified)
{
    return false;
}
