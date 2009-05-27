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
     *  FrameWindow
     */
    class UI_API FrameWindow : public Window
    {
        HARE_DECLARE_DYNAMIC_CLASS(FrameWindow)

    public:
        FrameWindow();
        FrameWindow(Window* parent);

        virtual bool layout(); 

        virtual void render(ThemePackage* themes);

        bool isSizingEnabled() const { return sizingEnabled; }
        bool isMovingEnabled() const { return movingEnabled; }


    protected:
        void onMouseMove(MouseEvent& event);
        void onMouseLButtonDown(MouseEvent& event);
        void onMouseLButtonUp(MouseEvent& event);

    protected:
        bool  sizingEnabled;
        bool  movingEnabled;
        float resizeBorder;
        float titleHeight;

        bool sizing;
        PointF sizingPos;

        bool moving;
        PointF movingPos;

        HARE_DECLARE_EVENT_TABLE()
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