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
#include "UVRCMIMEHandler.h"
#include "UVRCWizard.h"

//////////////////////////////////////////////////////////////////////////
static const PluginInfo uvrcMIMEHandler = {
    wxT("UVRCMIMEHandler"), 
    wxT("UVRCMIMEHandler"),
    wxT("This is a plugin for uvrc editing.") };

PluginRegistrant<UVRCMIMEHandler> regUVRCMIMEHandler(uvrcMIMEHandler);

//////////////////////////////////////////////////////////////////////////
static const PluginInfo uvrcWizard = {
    wxT("UVRCWizard"), 
    wxT("UVRCWizard"),
    wxT("This is a plugin for uvrc creating.") };

PluginRegistrant<UVRCWizard> regUVRCWizard(uvrcWizard);
//////////////////////////////////////////////////////////////////////////