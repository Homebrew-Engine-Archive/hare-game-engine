//***************************************************************
//  File:    ProjectExplorer.h
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
#ifndef PROJECTEXPLORER_H
#define PROJECTEXPLORER_H

#include "EditorPrerequisites.h"
#include "ProjectFile.h"
#include <wx/treectrl.h>


namespace hare_editor
{
    class EDITOR_API Project : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(Project)
    public:
        bool positionBreakPoints();
        ProjectFile::Ptr findFile(const String& name);

        String projectName;
        String debuggerName;
        ProjectFile::List files;

        wxTreeItemId treeNode;
    };

    class EDITOR_API Workspace : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(Workspace)
    public:
        Project::List projects;

        String activeProject;

        Project::Ptr findProject(const String& name);
    };

    class EDITOR_API ProjectExplorer : public wxPanel
    {
    public:
        ProjectExplorer(wxWindow *parent);
       ~ProjectExplorer();

        bool loadWorkspace(const wxString& dir);
        void saveWorkspace();

        void updateProjectFiles();

        void setProjectActive(Project* project);

        Project* getActiveProject();

    private:
        void createImageList();

        void onProjectFileActivated(wxTreeEvent& event);

    private:
        wxTreeCtrl* projectTree;
        
        wxImageList* imageList;

        wxTreeItemId root;

        Workspace::Ptr workspace;

        Project* activeProject;

        DECLARE_EVENT_TABLE()
    };
}

#endif
