//***************************************************************
//  File:    MouseCursor.h
//  Data:    05/27/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef _MOUSECURSOR_H_
#define _MOUSECURSOR_H_

#include "UIPrerequisites.h"

namespace hare
{
    enum CursorState
    {
        Cursor_Arrow = 0,
        Cursor_Moving,
        Cursor_SizingNESW,
        Cursor_SizingNS,
        Cursor_SizingNWSE,
        Cursor_SizingWE,

        Cursor_MAX,

        Cursor_None = -1,
    };

    class UI_API MouseCursor : public Singleton<MouseCursor>
    {
        HARE_DECLARE_SINGLETON(MouseCursor)
    public:
        MouseCursor() : currState(Cursor_None) {}

        void setCursor(CursorState state)
        {
            currState = state;
        }
        
        CursorState getCursor() const
        {
            return currState;
        }
        
        void setCursorSprite(CursorState state, Sprite* sprite)
        {
            if (state == Cursor_None)
                return;

            cursors[state] = sprite;
        }
        
        Sprite* getCursorSprite(CursorState state)
        {
            return cursors[state];
        }

        Sprite* getCursorSprite()
        {
            if (currState == Cursor_None)
                return NULL;

            return cursors[currState];
        }

    protected:
        Sprite::Ptr cursors[Cursor_MAX];
        CursorState currState;
    };
}

#endif
