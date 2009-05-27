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
    HARE_IMPLEMENT_DYNAMIC_CLASS(FrameWindow, Window, 0)
    {
        HARE_META(sizingEnabled, bool)
        HARE_META(movingEnabled, bool)
        HARE_META(resizeBorder, float)
        HARE_META(titleHeight, float)
    }

    HARE_BEGIN_EVENT_TABLE(FrameWindow, Window)
        HARE_EVT_MOTION(FrameWindow::onMouseMove)
        HARE_EVT_LEFT_DOWN(FrameWindow::onMouseLButtonDown)
        HARE_EVT_LEFT_UP(FrameWindow::onMouseLButtonUp)
    HARE_END_EVENT_TABLE()

    FrameWindow::FrameWindow() : 
        sizingEnabled(true), movingEnabled(true), resizeBorder(3), titleHeight(30)
    {
    }

    FrameWindow::FrameWindow(Window* parent) : Window(parent),
        sizingEnabled(true), movingEnabled(true), resizeBorder(3), titleHeight(30)
    {
    }

    bool FrameWindow::layout()
    {
        if (windowSizer)
        {
            windowSizer->setDimension(PointF(0, 0), size);
        }

        return true;
    }

    void FrameWindow::render(ThemePackage* themes)
    {
        Window::render(themes);
    }

    void FrameWindow::onMouseMove(MouseEvent& event)
    {
        if (isSizingEnabled())
        {
            PointF localMousePos(screenToWindow(*this, event.getPosition()));

            if (sizing)
            {
                // do sizing
                //setSize(getPosition() + localMousePos - draggingPos);
            }
            else if (moving)
            {
                // do dragging
                setPosition(getPosition() + localMousePos - movingPos);
                layout();
            }
            else
            {
                // do update cursor
            }

        }

    }

    void FrameWindow::onMouseLButtonDown(MouseEvent& event)
    {
        if (isSizingEnabled() || isMovingEnabled())
        {
            PointF localPos(screenToWindow(*this, event.getPosition()));

            RectF outer(0, 0, size.cx, size.cy);

            if (isSizingEnabled())
            {
                RectF inner = outer;
                inner.deflate(resizeBorder, resizeBorder, resizeBorder, resizeBorder);
                if (outer.isPointIn(localPos) && !inner.isPointIn(localPos))
                {
                    captureMouse();
                    sizing = true;
                    sizingPos = localPos;
                    return;
                }
            }

            if (isMovingEnabled())
            {
                RectF titleRect(outer);
                titleRect.maxY = titleRect.minX + titleHeight;
                if (titleRect.isPointIn(localPos))
                {
                    captureMouse();
                    moving = true;
                    movingPos = localPos;
                    return;
                }
            }
        }
    }

    void FrameWindow::onMouseLButtonUp(MouseEvent& event)
    {
        if (getCapture() == this)
        {
            releaseMouse();
            sizing = false;
            moving = false;
        }
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