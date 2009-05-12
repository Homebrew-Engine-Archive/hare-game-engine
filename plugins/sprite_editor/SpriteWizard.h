//***************************************************************
//  File:    SpriteWizard.h
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _SPRITEWIZARD_H_
#define _SPRITEWIZARD_H_

class SpriteWizard : public WizardPlugin
{
public:
    SpriteWizard();

    virtual wxString getFolder() const;
    virtual const wxBitmap& getBitmap(int index) const;
    virtual wxString getTitle(int index) const;
    virtual wxString getDesc(int index) const;
    virtual int getCount() const;
    virtual Object* wizard(int index);

private:
    wxBitmap bitmap;
    wxBitmap wizardImageSpriteBitmap;
    wxBitmap wizardAnimationBitmap;
    wxBitmap wizardComponentBitmap;
};

#endif