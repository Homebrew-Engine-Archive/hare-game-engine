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
#include "ThemeMIMEHandler.h"

static const PluginInfo themeMIMEHandler = {
    wxT("ThemeMIMEHandler"), 
    wxT("ThemeMIMEHandler"),
    wxT("This is a plugin for theme editing.") };

PluginRegistrant<ThemeMIMEHandler> regThemeMIMEHandler(themeMIMEHandler);