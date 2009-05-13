//***************************************************************
//  File:    UVRectState.cpp
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
#include "PCH.h"
#include "UVRectState.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(RectState, Object, 0)
    {
        HARE_META(name, String)
        HARE_META_F(rect, RectUV, propHide)
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(UVEditorState, Object, 0)
    {
        HARE_OBJ_LIST(rects, RectState)
        HARE_OBJ_F(mtrl, Material, propImport)
    }
}

