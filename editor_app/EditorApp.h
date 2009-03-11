//***************************************************************
//  File:    EditorApp.h
//  Date:    10/20/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _EDITORAPP_H_
#define _EDITORAPP_H_

#include <wx/app.h>

class EditorApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

private:
    void InitLocale();
    wxLocale locale;
};

#endif //_EDITORAPP_H_
