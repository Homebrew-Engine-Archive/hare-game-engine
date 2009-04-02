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

PluginRegistrant<FontMIMEHandler> regFontMIMEHandler(fontMIMEHandler);
