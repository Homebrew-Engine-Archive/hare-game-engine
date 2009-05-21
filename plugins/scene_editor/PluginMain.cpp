//***************************************************************
//  File:    PluginMain.cpp
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
#include "SceneWizard.h"

//////////////////////////////////////////////////////////////////////////
static const PluginInfo sceneMIMEHandler = {
    wxT("sceneMIMEHandler"), 
    wxT("sceneMIMEHandler"),
    wxT("This is a plugin for scene editing.") };

PluginRegistrant<SceneMIMEHandler> regSceneMIMEHandler(sceneMIMEHandler);
//////////////////////////////////////////////////////////////////////////

static const PluginInfo sceneWizard = {
    wxT("sceneWizard"), 
    wxT("sceneWizard"),
    wxT("This is a plugin for scene creating.") };

PluginRegistrant<SceneWizard> regSceneWizard(sceneWizard);
//////////////////////////////////////////////////////////////////////////
