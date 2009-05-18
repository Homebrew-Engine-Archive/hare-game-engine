//***************************************************************
//  File:    FrameWindow.h
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
#ifndef _FRAMEWINDOW_H_
#define _FRAMEWINDOW_H_

#include "UIPrerequisites.h"
#include "Window.h"
#include "Theme.h"

namespace hare
{
    /*
     *  Titlebar
     */
    class UI_API Titlebar : public Window
    {
        HARE_DECLARE_DYNAMIC_CLASS(Titlebar)
    };

    /*
     *  FrameWindow
     */
    class UI_API FrameWindow : public Window
    {
        HARE_DECLARE_DYNAMIC_CLASS(FrameWindow)

    public:
        FrameWindow();
        FrameWindow(Window* parent);

        Titlebar* getTitlebar();

    protected:
        Titlebar::Ptr titlebar;
    };

    /*
     *  FrameWindowTheme
     */
    class UI_API FrameWindowTheme : public Theme
    {
        HARE_DECLARE_DYNAMIC_CLASS(FrameWindowTheme)

    public:
        virtual ClassInfo* getWindowClass()
        {
            return &FrameWindow::CLASS_INFO;
        }

        virtual void render(Window* window);

    protected:
        RectUV rect;
        RectUV rectInner;
    };
}

#endif