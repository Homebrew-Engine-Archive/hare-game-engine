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
#include "MtrlMIMEHandler.h"
#include "MtrlWizard.h"

//////////////////////////////////////////////////////////////////////////
static const PluginInfo mtrlMIMEHandler = {
    wxT("MtrlMIMEHandler"), 
    wxT("MtrlMIMEHandler"),
    wxT("This is a plugin for material editing.") };

PluginRegistrant<MtrlMIMEHandler> regMtrlMIMEHandler(mtrlMIMEHandler);

//////////////////////////////////////////////////////////////////////////
static const PluginInfo mtrlWizard = {
    wxT("MtrlWizard"), 
    wxT("MtrlWizard"),
    wxT("This is a plugin for material creating.") };

PluginRegistrant<MtrlWizard> regMtrlWizard(mtrlWizard);
//////////////////////////////////////////////////////////////////////////
