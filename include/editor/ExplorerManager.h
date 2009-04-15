//***************************************************************
//  File:    ExplorerManager.h
//  Date:    10/23/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "EditorPrerequisites.h"
#include "TManager.h"
#include <wx/treectrl.h>

namespace hare
{
    class EDITOR_API ExplorerManager : public TManager<ExplorerManager>
    {
        friend class TManager<ExplorerManager>;

    public:
        wxFlatNotebook* getNotebook() { return notebook; }
        ProjectExplorer* getProjectExplorer() { return projectExplorer; }

        void bindProperty(const wxString& name, Object* object);
        void removeAllProperties();

    private:
        wxFlatNotebook* notebook;
        wxPropertyGridManager* propGridManager;
        FileSystemExplorer* fsExplorer;
        ProjectExplorer* projectExplorer;

    private:
        void createFileSystemPage();
        void createPropertyPage();
        void createProjectPage();

    private:
        ExplorerManager();
        ~ExplorerManager();
    };
}

#endif
