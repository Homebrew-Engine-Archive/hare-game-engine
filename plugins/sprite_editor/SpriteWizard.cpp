//***************************************************************
//  File:    SpriteWizard.cpp
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
#include "SpriteWizard.h"
#include "ImageSpriteWizard.h"
#include "SpriteMIMEHandler.h"


const ClassInfo* spriteClasses[] = {
    &ImageSprite::CLASS_INFO,
    &ComponentSprite::CLASS_INFO,
    &AnimationSprite::CLASS_INFO,
    &ParticleSprite::CLASS_INFO,
};

SpriteWizard::SpriteWizard()
{
    bitmap = Manager::loadBitmap(Manager::getInstancePtr()->getAppDir() + wxT("/resources.zip#zip:file32.png"));
    wizardImageSpriteBitmap = Manager::loadBitmap(Manager::getInstancePtr()->getAppDir() + wxT("/resources.zip#zip:mtrl_edit.png"));
    wizardAnimationBitmap = wizardImageSpriteBitmap;
    wizardComponentBitmap = wizardImageSpriteBitmap;
}

wxString SpriteWizard::getFolder() const
{
    return wxT("Sprites");
}

const wxBitmap& SpriteWizard::getBitmap(int index) const
{
    return bitmap;
}

wxString SpriteWizard::getTitle(int index) const
{
    return wxString::FromUTF8(spriteClasses[index]->className);
}

wxString SpriteWizard::getDesc(int index) const
{
    return wxString::FromUTF8(spriteClasses[index]->className);
}

int SpriteWizard::getCount() const
{
    return sizeof(spriteClasses)/sizeof(spriteClasses[0]); 
}

Object* SpriteWizard::wizard(int index)
{
    switch (index)
    {
    case 0:
        {
            ImageSpriteWizard wizard(Manager::getInstancePtr()->getAppWindow(), _("ImageSprite Wizard [create material]"),
                wizardImageSpriteBitmap);

            if (wizard.RunWizard(wizard.GetFirstPage())){
                Object::Ptr object = spriteClasses[index]->createObject();

                if (object){
                    EditorPlugin* plugin = EditorPluginManager::getInstancePtr()->findPluginByName(wxT("SpriteMIMEHandler"));
                    if (plugin && plugin->getType() == EPT_MIMEHandler){
                        ImageSprite* sprite = (ImageSprite*)object.pointer();
                        sprite->loadFromMaterial(wizard.getMaterial());
                        sprite->setUV(wizard.getRectUV());
                        SpriteMIMEHandler* handler = (SpriteMIMEHandler*)plugin;
                        handler->newPageImageSprite(sprite);
                    }
                }
            }
        }
        break;
    case 1:
        {
            Object::Ptr object = spriteClasses[index]->createObject();

            if (object){
                EditorPlugin* plugin = EditorPluginManager::getInstancePtr()->findPluginByName(wxT("SpriteMIMEHandler"));
                if (plugin && plugin->getType() == EPT_MIMEHandler){
                    ComponentSprite* sprite = (ComponentSprite*)object.pointer();
                    SpriteMIMEHandler* handler = (SpriteMIMEHandler*)plugin;
                    handler->newPageComponentSprite(sprite);
                }
            } 
        }
        break;
    case 2:
        {
            Object::Ptr object = spriteClasses[index]->createObject();

            if (object){
                EditorPlugin* plugin = EditorPluginManager::getInstancePtr()->findPluginByName(wxT("SpriteMIMEHandler"));
                if (plugin && plugin->getType() == EPT_MIMEHandler){
                    AnimationSprite* sprite = (AnimationSprite*)object.pointer();
                    SpriteMIMEHandler* handler = (SpriteMIMEHandler*)plugin;
                    handler->newPageAnimationSprite(sprite);
                }
            }
        }
        break;
    case 3:
        {
            Object::Ptr object = spriteClasses[index]->createObject();

            if (object){
                EditorPlugin* plugin = EditorPluginManager::getInstancePtr()->findPluginByName(wxT("SpriteMIMEHandler"));
                if (plugin && plugin->getType() == EPT_MIMEHandler){
                    ParticleSprite* sprite = (ParticleSprite*)object.pointer();
                    SpriteMIMEHandler* handler = (SpriteMIMEHandler*)plugin;
                    handler->newPageParticleSprite(sprite);
                }
            } 
        }
        break;
    }


    return NULL;
}