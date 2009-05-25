//***************************************************************
//  File:    CheckBox.h
//  Data:    05/25/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "UIPrerequisites.h"
#include "Button.h"
#include "Theme.h"

namespace hare
{
    class UI_API CheckBox : public Button
    {
        HARE_DECLARE_DYNAMIC_CLASS(CheckBox)
    public:
        CheckBox() : checked(false) {};
        CheckBox(Window* parent) : Button(parent), checked(false) {}

        bool isChecked() const
        {
            return checked;
        }
        virtual bool check(bool check)
        {
            checked = check;
            return true;
        }

    protected:
        void onMouseClick(MouseEvent& event);

    protected:
        bool checked;

        HARE_DECLARE_EVENT_TABLE()
    };

    class UI_API CheckBoxTheme : public Theme
    {
        HARE_DECLARE_DYNAMIC_CLASS(CheckBoxTheme)
    public:
        virtual ClassInfo* getWindowClass()
        {
            return &CheckBox::CLASS_INFO;
        }
        virtual void render(Window* window);

    protected:
        RectUV rectNormal;
        RectUV rectNormalChecked;
        RectUV rectHover;
        RectUV rectHoverChecked;
        RectUV rectPushed;
        RectUV rectPushedChecked;
        RectUV rectDisabled;
        RectUV rectDisabledChecked;
    };
}

#endif