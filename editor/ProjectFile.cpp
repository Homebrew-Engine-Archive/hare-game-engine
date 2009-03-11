//***************************************************************
//  File:    ProjectFile.cpp
//  Data:    02/11/2009
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
#include "ProjectFile.h"

namespace hare_editor
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(ProjectFile, Object, 0)
    {
        HARE_META(fileName, String)
        HARE_META(editorOpen, bool)
        HARE_META(editorPos, s32)
        HARE_META(editorTopLine, s32)
        HARE_META(editorTabPos, s32)
        HARE_META_ARRAY(breakPoints, s32)
    }

    ProjectFile::ProjectFile()
        : editorOpen(false), editorPos(0), editorTopLine(0), editorTabPos(0)
    {
    }
}
