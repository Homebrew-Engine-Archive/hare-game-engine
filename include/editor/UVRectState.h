//***************************************************************
//  File:    UVRectState.h
//  Data:    05/13/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#include "EditorPrerequisites.h"

namespace hare
{
    typedef TRect<float> RectUV;

    /* 
    * RectState
    */
    class EDITOR_API RectState : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(RectState)
    public:

        enum EditState
        {
            State_None      = 1 << 0,
            State_Selected  = 1 << 1,
            State_Focused   = 1 << 2 | State_Selected,
        };

        bool hasFlag(EditState flag)
        {
            return (state & flag) == flag;
        }

        RectState() : name("noname"), rect(0, 0, 1, 1), state(State_None)
        {
        }

    public:
        String name;
        RectUV rect;
        EditState state;
    };

    /* 
    * UVEditorState
    */
    class EDITOR_API UVEditorState : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(UVEditorState)
    public:
        PointF offset;          // Offset from window center to image center
        float scale;
        RectState::List rects;
        Material::Ptr mtrl;

        UVEditorState() : scale(1.0f), offset(0.0f, 0.0f)
        {
        }
    };
}

