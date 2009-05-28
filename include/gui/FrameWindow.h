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

        void updateCursor(const PointF& pt);

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
        bool moving;
        PointF mouseDownPos;
        
        enum SizingLocation 
        {
            SizingNone,			//!< Position is not a sizing location.
            SizingTopLeft,		//!< Position will size from the top-left.
            SizingTopRight,		//!< Position will size from the top-right.
            SizingBottomLeft,	//!< Position will size from the bottom left.
            SizingBottomRight,	//!< Position will size from the bottom right.
            SizingTop,			//!< Position will size from the top.
            SizingLeft,			//!< Position will size from the left.
            SizingBottom,		//!< Position will size from the bottom.
            SizingRight         //!< Position will size from the right.
        };

        SizingLocation getSizingBorderAtPoint(const PointF& pt) const;
        
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