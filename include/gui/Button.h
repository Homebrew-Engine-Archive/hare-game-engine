//***************************************************************
//  File:    Button.h
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
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "UIPrerequisites.h"
#include "Window.h"
#include "Theme.h"

namespace hare
{
    class UI_API Button : public Window
    {
        HARE_DECLARE_DYNAMIC_CLASS(Button)
    public:
        Button() : pushed(false), hovering(false) {};
        Button(Window* parent) : Window(parent), pushed(false), hovering(false) {}

        bool isPushed() const
        {
            return pushed;
        }
        bool isHovering() const
        {
            return hovering;
        }

    protected:
        void updateState(const PointF& pos);
        void onMouseMove(MouseEvent& event);
        void onMouseLButtonDown(MouseEvent& event);
        void onMouseLButtonUp(MouseEvent& event);
        void onMouseLeaves(MouseEvent& event);

    protected:
        bool pushed;
        bool hovering;

        HARE_DECLARE_EVENT_TABLE()
    };

    class UI_API ButtonTheme : public Theme
    {
        HARE_DECLARE_DYNAMIC_CLASS(ButtonTheme)
    public:
        virtual ClassInfo* getWindowClass()
        {
            return &Button::CLASS_INFO;
        }
        virtual void render(Window* window);

    protected:
        RectUV rectNormal;
        RectUV rectNormalInner;
        RectUV rectHover;
        RectUV rectHoverInner;
        RectUV rectPushed;
        RectUV rectPushedInner;
        RectUV rectDisabled;
        RectUV rectDisabledInner;
    };
}

#endif