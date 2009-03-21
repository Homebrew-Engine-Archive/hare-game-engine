//***************************************************************
//  File:    harecanvas.cpp
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
#include "wx/harecanvas.h"

const wxChar* wxHareCanvasNameStr = wxT("HareCanvas");

wxHareCanvas::wxHareCanvas(wxWindow *parent, wxWindowID id, bool hasZBuffer, const wxPoint& pos,
        const wxSize& size, long style, const wxString& name) : wxWindow()
{
    renderWindow = NULL;

    Create(parent, id, pos, size, style, name);

    WindowParams params;
    params.hwnd = (u32)GetHWND();
    params.bFullScreen = false;
    params.width = size.GetWidth() > 0 ? size.GetWidth() : 800;
    params.height = size.GetHeight() > 0 ? size.GetHeight() : 600;
    params.bZbuffer = hasZBuffer;
    params.title = name.ToUTF8().data();

    renderWindow = getHareApp()->createRenderWindow(params);
}

wxHareCanvas::~wxHareCanvas()
{
    if (renderWindow)
    {
        renderWindow->destoryWindow();
    }
    renderWindow = NULL;
}

bool wxHareCanvas::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, 
        const wxSize& size, long style, const wxString& name)
{
    wxCHECK_MSG(parent, false, wxT("can't create wxWindow without parent"));

    return wxWindow::Create(parent, id, pos, size, style, name);
}