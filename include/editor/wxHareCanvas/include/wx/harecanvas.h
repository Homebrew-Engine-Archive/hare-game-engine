//***************************************************************
//  File:    harecanvas.h
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
#ifndef HARECANVAS_H
#define HARECANVAS_H

#include <wx/window.h>
#include <wx/string.h>

#include "graphics/Graphics.h"
using namespace hare;

#ifdef WXMAKINGDLL_HARE_CANVAS
#    define WXDLLIMPEXP_HARE_CANVAS WXEXPORT
#elif defined(WXUSINGDLL_HARE_CANVAS)
#    define WXDLLIMPEXP_HARE_CANVAS WXIMPORT
#else
#    define WXDLLIMPEXP_HARE_CANVAS
#endif

extern WXDLLIMPEXP_HARE_CANVAS const wxChar* wxHareCanvasNameStr;

class WXDLLIMPEXP_HARE_CANVAS wxHareCanvas : public wxWindow
{
public:
    wxHareCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, bool hasZBuffer = true,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxHareCanvasNameStr);

    virtual ~wxHareCanvas();

    bool Create(wxWindow *parent, wxWindowID id,
        const wxPoint& pos, const wxSize& size,
        long style, const wxString& name);

    RenderWindow* getRenderWindow() const
    {
        return renderWindow;
    }

protected:

    void onEraseBackground(wxEraseEvent& event);

    RenderWindow* renderWindow;

    DECLARE_EVENT_TABLE()
};

#endif