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
#include "TextEditorPage.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(ProjectFile, Object, 0)
    {
        HARE_META(fileName, String)
        HARE_META(editorOpen, bool)
        HARE_META(editorPos, int32)
        HARE_META(editorTopLine, int32)
        HARE_META(editorTabPos, int32)
        HARE_META_ARRAY(breakPoints, int32)
    }

    ProjectFile::ProjectFile()
        : editorOpen(false), editorPos(0), editorTopLine(0), editorTabPos(0)
    {
    }

    bool ProjectFile::addBreakPoint(int line)
    {
        if (std::find(breakPoints.begin(), breakPoints.end(), line) == breakPoints.end())
            breakPoints.push_back(line);

        return true;
    }

    bool ProjectFile::removeBreakPoint(int line)
    {
        std::vector<int32>::iterator it = std::find(breakPoints.begin(), breakPoints.end(), line);

        if (it != breakPoints.end())
            breakPoints.erase(it);

        return true;
    }

    bool ProjectFile::updateBreakPoint(TextEditorPage* page)
    {
        breakPoints.clear();

        if (page)
        {
            for (int i = 0; i < page->getLineCount(); ++i)
            {
                if (page->lineHasMarker(BREAKPOINT_MARKER, i))
                {
                    breakPoints.push_back(i);
                }
            }

            return true;
        }

        return false;
    }
}
