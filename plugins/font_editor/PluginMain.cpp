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
#include "FontMIMEHandler.h"

static const PluginInfo fontMIMEHandler = {
    wxT("FontMIMEHandler"), 
    wxT("FontMIMEHandler"),
    wxT("This is a plugin for font editing.") };

static const PluginInfo fontWizard = {
    wxT("FontWizard"), 
    wxT("FontWizard"),
    wxT("This is a plugin for font creation.") };

PluginRegistrant<FontMIMEHandler> regFontMIMEHandler(fontMIMEHandler);
//PluginRegistrant<FontWizard> regFontWizard(fontWizard);