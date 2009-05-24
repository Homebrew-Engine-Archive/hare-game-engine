//***************************************************************
//  File:    Button.cpp
//  Data:    04/22/2009
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
#include "Button.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(Button, Window, 0)
    {
    }

    HARE_BEGIN_EVENT_TABLE(Button, Window)
        HARE_EVT_MOTION(Button::onMouseMove)
        HARE_EVT_LEFT_DOWN(Button::onMouseLButtonDown)
        HARE_EVT_LEFT_UP(Button::onMouseLButtonUp)
        HARE_EVT_LEAVE_WINDOW(Button::onMouseLeaves)
    HARE_END_EVENT_TABLE()

    void Button::updateState(const PointF& pos)
    {
        hovering = (getCapture() == this || hitTest(pos));
    }

    void Button::onMouseMove(MouseEvent& event)
    {
        updateState(event.getPosition());
    }

    void Button::onMouseLButtonDown(MouseEvent& event)
    {
        captureMouse();
        pushed = true;
        updateState(event.getPosition());
    }

    void Button::onMouseLButtonUp(MouseEvent& event)
    {
        releaseMouse();
        pushed = false;
    }

    void Button::onMouseLeaves(MouseEvent& event)
    {
        hovering = false;
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(ButtonTheme, Theme, 0)
    {
        HARE_GUI_UV(rectNormal)
        HARE_GUI_UV(rectNormalInner)
        HARE_GUI_UV(rectHover)
        HARE_GUI_UV(rectHoverInner)
        HARE_GUI_UV(rectPushed)
        HARE_GUI_UV(rectPushedInner)
        HARE_GUI_UV(rectDisabled)
        HARE_GUI_UV(rectDisabledInner)
    }

    void ButtonTheme::render(Window* window)
    {
        if (!window)
            return;

        RectF* rect = NULL;
        RectF* rectInner = NULL;
        Button* button = (Button*)window;

        if (!button->isEnabled())
        {
            rect = &rectDisabled;
            rectInner = &rectDisabledInner;
        }
        else if (button->isPushed())
        {
            rect = &rectPushed;
            rectInner = &rectPushedInner;
        }
        else if (button->isHovering())
        {
            rect = &rectHover;
            rectInner = &rectHoverInner;
        }
        else
        {
            rect = &rectNormal;
            rectInner = &rectNormalInner;
        }

        drawThemeInternal(mtrl, window->getPixelRect(), *rect, *rectInner);
    }
}

