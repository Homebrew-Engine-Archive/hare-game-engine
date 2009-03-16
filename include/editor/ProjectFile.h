//***************************************************************
//  File:    ProjectFile.h
//  Data:    02/10/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "EditorPrerequisites.h"
#include <wx/filename.h>
#include <wx/treebase.h>

namespace hare_editor
{
    class ProjectFile : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(ProjectFile)
    public:
        ProjectFile();

        bool addBreakPoint(int line);
        bool removeBreakPoint(int line);

    public:
        String fileName;
        bool editorOpen;
        s32 editorPos;
        s32 editorTopLine;
        s32 editorTabPos;
        std::vector<s32> breakPoints;

        wxFileName fname;
        wxTreeItemId treeId;
    };
}

#endif
