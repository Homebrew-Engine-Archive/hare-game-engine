//***************************************************************
//  File:    SceneWizard.h
//  Date:    5/19/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _SCENEWIZARD_H_
#define _SCENEWIZARD_H_

class SceneWizard : public WizardPlugin
{
public:
    SceneWizard();

    virtual wxString getFolder() const;
    virtual const wxBitmap& getBitmap(int index) const;
    virtual wxString getTitle(int index) const;
    virtual wxString getDesc(int index) const;
    virtual int getCount() const;
    virtual Object* wizard(int index);

private:
    wxBitmap bitmap;
};

#endif
