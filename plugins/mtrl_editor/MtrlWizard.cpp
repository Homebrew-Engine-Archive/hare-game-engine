//***************************************************************
//  File:    MtrlWizard.cpp
//  Data:    04/29/2009
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
#include "MtrlWizard.h"

const ClassInfo* mtrlClasses[] = {
    &TextureMtrl::CLASS_INFO,
    &PannerMod::CLASS_INFO,
    &ScalerMod::CLASS_INFO,
    &RotatorMod::CLASS_INFO,
    &SimpleShader::CLASS_INFO,
    &ParticleShader::CLASS_INFO,
};

MtrlWizard::MtrlWizard()
{
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
    bitmap.LoadFile(fullPath + wxT("file32.png"), wxBITMAP_TYPE_PNG);
}

wxString MtrlWizard::getFolder() const
{
    return wxT("Materials");
}

const wxBitmap& MtrlWizard::getBitmap(int index) const
{
    return bitmap;
}

wxString MtrlWizard::getTitle(int index) const
{
    return wxString::FromUTF8(mtrlClasses[index]->className);
}

wxString MtrlWizard::getDesc(int index) const
{
    return wxString::FromUTF8(mtrlClasses[index]->className);
}

int MtrlWizard::getCount() const
{
    return sizeof(mtrlClasses) / sizeof(mtrlClasses[0]);
}

Object* MtrlWizard::wizard(int index)
{
    return mtrlClasses[index]->createObject();
}
