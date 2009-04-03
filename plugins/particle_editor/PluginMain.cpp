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
#include "ParticleMIMEHandler.h"

static const PluginInfo particleMIMEHandler = {
    wxT("ParticleMIMEHandler"), 
    wxT("ParticleMIMEHandler"),
    wxT("This is a plugin for particle editing.") };

PluginRegistrant<ParticleMIMEHandler> regParticleMIMEHandler(particleMIMEHandler);