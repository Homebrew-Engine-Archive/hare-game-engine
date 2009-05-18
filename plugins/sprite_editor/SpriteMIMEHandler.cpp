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
#include "ParticleSpritePage.h"
#include "ComponentSpritePage.h"

SpriteMIMEHandler::SpriteMIMEHandler()
{
    
}


bool SpriteMIMEHandler::canHandle(const wxString& filename) const
{
    if (!filename.Lower().EndsWith(wxT(".sprite")))
        return false;

    Object::Ptr obj = Object::importObject(filename.ToUTF8().data());
    if (!obj)
        return false;

    if (obj->getClassInfo()->isDerivedFrom(&ImageSprite::CLASS_INFO))
        return true;
    if (obj->getClassInfo()->isDerivedFrom(&ComponentSprite::CLASS_INFO))
        return true;
    if (obj->getClassInfo()->isDerivedFrom(&AnimationSprite::CLASS_INFO))
        return true;
    if (obj->getClassInfo()->isDerivedFrom(&ParticleSprite::CLASS_INFO))
        return true;

    return false;
}

bool SpriteMIMEHandler::openFile(const wxString& filename)
{
    Object::Ptr obj = Object::importObject(filename.ToUTF8().data());

    if (!obj)
        return false;

    if (obj->getClassInfo()->isDerivedFrom(&ImageSprite::CLASS_INFO)){
        return newPageImageSprite((ImageSprite*)obj.pointer(), false);
    }

    if (obj->getClassInfo()->isDerivedFrom(&ComponentSprite::CLASS_INFO)){
        return newPageComponentSprite((ComponentSprite*)obj.pointer(), false);
    }

    if (obj->getClassInfo()->isDerivedFrom(&AnimationSprite::CLASS_INFO)){
        return newPageAnimationSprite((AnimationSprite*)obj.pointer(), false);
    }

    if (obj->getClassInfo()->isDerivedFrom(&ParticleSprite::CLASS_INFO)){
        return newPageParticleSprite((ParticleSprite*)obj.pointer(), false);
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
    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    epm->getNotebook()->Freeze();
    ComponentSpritePage* page = new ComponentSpritePage(epm->getNotebook(), this);
    epm->addEditorPage(page);
    epm->getNotebook()->Thaw();

    page->setComponentSprite(component);

    if (isModified)
        page->setModified(isModified);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return true;
}

bool SpriteMIMEHandler::newPageParticleSprite(ParticleSprite* particle, bool isModified)
{
    Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Freeze();
    ParticleEditorPage* page = new ParticleEditorPage(Manager::getInstancePtr()->getEditorPageManager()->getNotebook(), this, particle);
    Manager::getInstancePtr()->getEditorPageManager()->addEditorPage(page);
    Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Thaw();


    int index = Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->GetPageIndex(page);

    if (index != -1)
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->SetSelection(index);

    return page->isOk();
}
