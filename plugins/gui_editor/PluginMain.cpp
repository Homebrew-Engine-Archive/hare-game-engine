//***************************************************************
//  File:    PluginMain.cpp
//  Data:    03/20/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#include "PCH.h"
#include "GUIMIMEHandler.h"
#include "ThemeWizard.h"

static const PluginInfo guiMIMEHandler = {
    wxT("GUIMIMEHandler"), 
    wxT("GUIMIMEHandler"),
    wxT("This is a plugin for gui editing.") };

PluginRegistrant<GUIMIMEHandler> regGUIMIMEHandler(guiMIMEHandler);


static const PluginInfo themeWizard = {
    wxT("ThemeWizard"), 
    wxT("ThemeWizard"),
    wxT("This is a plugin for theme package creating.") };

PluginRegistrant<ThemeWizard> regThemeWizard(themeWizard);