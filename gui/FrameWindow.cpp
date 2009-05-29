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
#include "MouseCursor.h"

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
        sizingEnabled(true), movingEnabled(true), resizeBorder(5), titleHeight(30), mouseDownPos(0, 0),
        sizing(false), moving(false)
    {
    }

    FrameWindow::FrameWindow(Window* parent) : Window(parent),
        sizingEnabled(true), movingEnabled(true), resizeBorder(5), titleHeight(30), mouseDownPos(0, 0),
        sizing(false), moving(false)
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
                if (sizingEdge != SizingNone)
                {
                    float deltaX = localMousePos.x - mouseDownPos.x;
                    float deltaY = localMousePos.y - mouseDownPos.y;

                    RectF rect = getArea();

                    // NB : Moving left or top edge changes both position and size,
                    // moving right or bottom edge only changes size.
                    if (sizingEdge == SizingTopLeft || sizingEdge == SizingBottomLeft || sizingEdge == SizingLeft)
                    {
                        rect.minX += deltaX;
                    }
                    else if (sizingEdge == SizingTopRight || sizingEdge == SizingBottomRight || sizingEdge == SizingRight)
                    {
                        rect.maxX += deltaX;
                        mouseDownPos.x = localMousePos.x;
                    }

                    if (sizingEdge == SizingTopLeft || sizingEdge == SizingTop || sizingEdge == SizingTopRight)
                    {
                        rect.minY += deltaY;
                    }
                    else if (sizingEdge == SizingBottomLeft || sizingEdge == SizingBottom || sizingEdge == SizingBottomRight)
                    {
                        rect.maxY += deltaY;
                        mouseDownPos.y = localMousePos.y;
                    }

                    setArea(rect);
                    layout();
                }
            }
            else if (moving)
            {
                setPosition(getPosition() + localMousePos - mouseDownPos);
                layout();
            }
            else
            {
                updateCursor(localMousePos);
            }
        }
    }

    void FrameWindow::onMouseLButtonDown(MouseEvent& event)
    {
        if (isSizingEnabled() || isMovingEnabled())
        {
            PointF localPos(screenToWindow(*this, event.getPosition()));

            if (isSizingEnabled())
            {
                sizingEdge = getSizingBorderAtPoint(localPos);

                if (sizingEdge != SizingNone)
                {
                    captureMouse();
                    sizing = true;
                    mouseDownPos = localPos;
                    return;
                }
            }

            if (isMovingEnabled())
            {
                RectF titleRect(0, 0, size.cx, size.cy);
                titleRect.maxY = titleRect.minX + titleHeight;
                if (titleRect.isPointIn(localPos))
                {
                    captureMouse();
                    moving = true;
                    mouseDownPos = localPos;
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

    void FrameWindow::updateCursor(const PointF& pt)
    {
        switch (getSizingBorderAtPoint(pt))
        {
        case SizingTop:
        case SizingBottom:
            MouseCursor::getSingleton().setCursor(Cursor_SizingNS);
            break;

        case SizingLeft:
        case SizingRight:
            MouseCursor::getSingleton().setCursor(Cursor_SizingWE);
            break;

        case SizingTopLeft:
        case SizingBottomRight:
            MouseCursor::getSingleton().setCursor(Cursor_SizingNWSE);
            break;

        case SizingTopRight:
        case SizingBottomLeft:
            MouseCursor::getSingleton().setCursor(Cursor_SizingNESW);
            break;

        default:
            MouseCursor::getSingleton().setCursor(Cursor_Arrow);
        }
    }

    FrameWindow::SizingLocation FrameWindow::getSizingBorderAtPoint(const PointF& pt) const
    {
        RectF rect(0, 0, size.cx, size.cy);

        if (isSizingEnabled())
        {
            if (rect.isPointIn(pt))
            {
                rect.deflate(resizeBorder, resizeBorder, resizeBorder, resizeBorder);

                bool t = (pt.y <  rect.minY);
                bool b = (pt.y >= rect.maxY);
                bool l = (pt.x <  rect.minX);
                bool r = (pt.x >= rect.maxX);

                if (t && l)
                {
                    return SizingTopLeft;
                }
                else if (t && r)
                {
                    return SizingTopRight;
                }
                else if (b && l)
                {
                    return SizingBottomLeft;
                }
                else if (b && r)
                {
                    return SizingBottomRight;
                }
                else if (t)
                {
                    return SizingTop;
                }
                else if (b)
                {
                    return SizingBottom;
                }
                else if (l)
                {
                    return SizingLeft;
                }
                else if (r)
                {
                    return SizingRight;
                }
            }
        }

        return SizingNone;
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