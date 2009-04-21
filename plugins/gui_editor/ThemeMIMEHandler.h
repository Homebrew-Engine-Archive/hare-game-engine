//***************************************************************
//  File:    ThemeMIMEHandler.h
//  Data:    04/15/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef THEMEMIMEHANDLER_H
#define THEMEMIMEHANDLER_H

#include "UVEditorPage.h"
#include <wx/harecanvas.h>

class ThemeMIMEHandler;

class ThemeEditorPage : public UVEditorPage 
{
    friend class ThemeSceneListener;

public:
    ThemeEditorPage(wxWindow* parent, ThemeMIMEHandler* handler, Theme* theme);
    virtual ~ThemeEditorPage(); 

    bool isOk() const
    {
        return themePtr.pointer() != NULL;
    }

private:
    Theme::Ptr themePtr;
    ThemeMIMEHandler* mime;
    Button::Ptr button;

private:
    void onSize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onMouseMove(wxMouseEvent& event);

    DECLARE_ABSTRACT_CLASS(ThemeEditorPage)
};

class ThemeMIMEHandler : public MIMEHandlerPlugin
{
public:
    ThemeMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

private:

    friend class ThemeEditorPage;

    ThemeEditorPage* page;
};

#endif