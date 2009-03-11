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
    class ScriptPackage : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(ScriptPackage)
    public:
        bool positionBreakPoints();
        ProjectFile* findFile(const String& name);

        String packageName;
        ProjectFile::List files;
    };

    class Project : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(Project)
    public:
        ScriptPackage::List packages;

        ScriptPackage* findPackage(const String& name);
    };

    class TreeItemData : public wxTreeItemData
    {
    public:
        TreeItemData(ProjectFile* pf) : file(pf) {}
        ProjectFile *file;
    };

    class EDITOR_API ProjectExplorer : public wxPanel
    {
    public:
        ProjectExplorer(wxWindow *parent);
        ~ProjectExplorer();

        bool loadProject(const wxString& dir);
        void saveProject();

        void updateProjectFiles();

    private:
        void createImageList();

        void onProjectFileActivated(wxTreeEvent& event);

    private:
        wxTreeCtrl* projectTree;
        wxImageList* imageList;
        wxTreeItemId root;

        Project::Ptr project;

        DECLARE_EVENT_TABLE()
    };
}

#endif
