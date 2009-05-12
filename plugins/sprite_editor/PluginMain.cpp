//***************************************************************
//  File:    PluginMain.cpp
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
#include "SpriteWizard.h"

//////////////////////////////////////////////////////////////////////////
static const PluginInfo spriteMIMEHandler = {
    wxT("spriteMIMEHandler"), 
    wxT("spriteMIMEHandler"),
    wxT("This is a plugin for sprite editing.") };

PluginRegistrant<SpriteMIMEHandler> regSpriteMIMEHandler(spriteMIMEHandler);
//////////////////////////////////////////////////////////////////////////

static const PluginInfo spriteWizard = {
    wxT("spriteWizard"), 
    wxT("spriteWizard"),
    wxT("This is a plugin for sprite creating.") };

PluginRegistrant<SpriteWizard> regSpriteWizard(spriteWizard);
//////////////////////////////////////////////////////////////////////////
