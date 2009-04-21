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

static const PluginInfo mtrlMIMEHandler = {
    wxT("MtrlMIMEHandler"), 
    wxT("MtrlMIMEHandler"),
    wxT("This is a plugin for material editing.") };

PluginRegistrant<MtrlMIMEHandler> regMtrlMIMEHandler(mtrlMIMEHandler);