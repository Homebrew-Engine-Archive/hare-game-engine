//***************************************************************
//  File:    FrameWindow.cpp
//  Data:    05/17/2009
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
#include "FrameWindow.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(Titlebar, Window, 0)
    {
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(FrameWindow, Window, 0)
    {
        HARE_OBJ(titlebar, Titlebar)
    }

    FrameWindow::FrameWindow()
    {
    }

    FrameWindow::FrameWindow(Window* parent) : Window(parent)
    {
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(FrameWindowTheme, Theme, 0)
    {
        HARE_GUI_UV(rect)
        HARE_GUI_UV(rectInner)
    }

    void FrameWindowTheme::render(Window* window)
    {
        if (!window)
            return;

        drawThemeInternal(mtrl, window->getPixelRect(), rect, rectInner);
    }

}