//***************************************************************
//  File:    ImageSpriteWizard.h
//  Date:    5/11/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _IMAGESPRITEWIZARD_H_
#define _IMAGESPRITEWIZARD_H_

#include <wx/wizard.h>

class ImageSpriteWizardPage;

class ImageSpriteWizard : public wxWizard
{
public:
    ImageSpriteWizard(wxWindow* parent, const wxString& title, wxBitmap& bitmap);

    wxWizardPageSimple* GetFirstPage();

    Material::Ptr getMaterial();

    RectF getRectUV();

private:
    ImageSpriteWizardPage* page;
};

#endif

