//***************************************************************
//  File:    SceneWizard.cpp
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
#include "SceneWizard.h"
#include "SceneMIMEHandler.h"

const ClassInfo* sceneClasses[] = {
    &SceneManager::CLASS_INFO,
};

SceneWizard::SceneWizard()
{
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
    bitmap.LoadFile(fullPath + wxT("file32.png"), wxBITMAP_TYPE_PNG);
}

wxString SceneWizard::getFolder() const
{
    return wxT("Scene");
}

const wxBitmap& SceneWizard::getBitmap(int index) const
{
    return bitmap;
}

wxString SceneWizard::getTitle(int index) const
{
    return wxString::FromUTF8(sceneClasses[index]->className);
}

wxString SceneWizard::getDesc(int index) const
{
    return wxString::FromUTF8(sceneClasses[index]->className);
}

int SceneWizard::getCount() const
{
    return sizeof(sceneClasses)/sizeof(sceneClasses[0]); 
}

Object* SceneWizard::wizard(int index)
{
    switch(index)
    {
    case 0:
        {
            Object::Ptr obj = sceneClasses[index]->createObject();

            if (obj){
                EditorPlugin* plugin = PluginManager::getInstancePtr()->findPluginByName(wxT("sceneMIMEHandler"));
                if (plugin && plugin->getType() == EPT_MIMEHandler){
                    SceneManager* scene = (SceneManager*)obj.pointer();
                    SceneMIMEHandler* handler = (SceneMIMEHandler*)plugin;
                    handler->newPageScene(scene);
                }
            }
        }
        break;
    }

    return NULL;
}
