//***************************************************************
//  File:    ThemeWizard.h
//  Data:    05/14/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef THEMEWIZARD_H
#define THEMEWIZARD_H

class ThemeWizard : public WizardPlugin
{
public:
    ThemeWizard();

    virtual wxString getFolder() const;
    virtual const wxBitmap& getBitmap(int index) const;
    virtual wxString getTitle(int index) const;
    virtual wxString getDesc(int index) const;
    virtual int getCount() const;
    virtual Object* wizard(int index);

private:
    wxBitmap bitmap;
    wxBitmap wizardBitmap;
};

#endif