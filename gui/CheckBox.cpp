//***************************************************************
//  File:    CheckBox.cpp
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
#include "PCH.h"
#include "CheckBox.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(CheckBox, Button, 0)
    {
        HARE_META(checked, bool)
    }

    HARE_BEGIN_EVENT_TABLE(CheckBox, Button)
        HARE_EVT_LEFT_CLICK(CheckBox::onMouseClick)
    HARE_END_EVENT_TABLE()

    void CheckBox::onMouseClick(MouseEvent& event)
    {
        checked = !checked;
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(CheckBoxTheme, Theme, 0)
    {
        HARE_GUI_UV(rectNormal)
        HARE_GUI_UV(rectNormalChecked)
        HARE_GUI_UV(rectHover)
        HARE_GUI_UV(rectHoverChecked)
        HARE_GUI_UV(rectPushed)
        HARE_GUI_UV(rectPushedChecked)
        HARE_GUI_UV(rectDisabled)
        HARE_GUI_UV(rectDisabledChecked)
    }

    void CheckBoxTheme::render(Window* window)
    {
        if (!window)
            return;

        RectF* rect = NULL;
        CheckBox* checkBox = (CheckBox*)window;

        if (!checkBox->isEnabled())
        {
            if (checkBox->isChecked()) 
                rect = &rectDisabledChecked;
            else 
                rect = &rectDisabled;
        }
        else if (checkBox->isPushed())
        {
            if (checkBox->isChecked()) 
                rect = &rectPushedChecked;
            else 
                rect = &rectPushed;
        }
        else if (checkBox->isHovering())
        {
            if (checkBox->isChecked()) 
                rect = &rectHoverChecked;
            else 
                rect = &rectHover;
        }
        else
        {
            if (checkBox->isChecked()) 
                rect = &rectNormalChecked;
            else 
                rect = &rectNormal;
        }

        float w = (float)mtrl->getTextureMtrl()->getTexture()->getWidth();
        float h = (float)mtrl->getTextureMtrl()->getTexture()->getHeight();

        RectF pixelRect = *rect;
        pixelRect.scale(w, h);
        pixelRect.moveTo(window->getPixelRect().minX, window->getPixelRect().minY);
        getCanvas()->drawImage(pixelRect, mtrl, *rect);
    }
}

