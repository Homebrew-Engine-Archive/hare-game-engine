//***************************************************************
//  File:    ProjectExplorer.cpp
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
#include "PCH.h"
#include "ProjectExplorer.h"
#include "EditorManager.h"
#include "ConfigManager.h"
#include "EditorPageManager.h"
#include "TextEditorPage.h"
#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#include <wx/wxscintilla.h>
#include <wx/imaglist.h>
#include <wx/dir.h>

namespace hare
{
    class TreeItemData : public wxTreeItemData
    {
    public:
        TreeItemData(ProjectFile* pf) : file(pf) {}
        ProjectFile *file;
    };

    ProjectFile::Ptr Project::findFile(const String& name)
    {
        ProjectFile::List::iterator it = files.begin();
        for (; it != files.end(); ++it)
        {
            if ((*it)->fileName == name)
            {
                return *it;
            }
        }

        return 0;
    }

    Project::Ptr Workspace::findProject(const String& name)
    {
        Project::List::iterator it = projects.begin();
        for (; it != projects.end(); ++it)
        {
            if ((*it)->projectName == name)
                return *it;
        }

        return 0;
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(Project, Object, 0)
    {
        HARE_META(projectName, String)
        HARE_META(debuggerName, String)
        HARE_OBJ_LIST(files, ProjectFile)
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(Workspace, Object, 0)
    {
        HARE_OBJ_LIST(projects, Project)
        HARE_META(activeProject, String)
        HARE_META(activePage, int32)
    }

    int idProjectExplorer = wxNewId();

    BEGIN_EVENT_TABLE(ProjectExplorer, wxPanel)
        EVT_TREE_ITEM_ACTIVATED(idProjectExplorer, ProjectExplorer::onProjectFileActivated)
    END_EVENT_TABLE()

    ProjectExplorer::ProjectExplorer(wxWindow *parent)
        : wxPanel(parent), imageList(0), activeProject(0)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        projectTree = new wxTreeCtrl(this, idProjectExplorer, wxDefaultPosition, wxDefaultSize,
            wxTR_HAS_BUTTONS | wxNO_BORDER);

        createImageList();

        projectTree->SetImageList(imageList);
        root = projectTree->AddRoot(_("Scripts"), 0, 0);

        sizer->Add(projectTree, 1, wxEXPAND);
        SetSizer(sizer);
    }

    void ProjectExplorer::onProjectFileActivated(wxTreeEvent& event)
    {
        wxTreeItemId id = projectTree->GetSelection();
        TreeItemData* dat = (TreeItemData*)projectTree->GetItemData(id);

        if (!dat)
        {
            wxString projName = projectTree->GetItemText(id);

            if (workspace->projects.size() > 0)
            {
                workspace->activeProject = projName.ToUTF8().data();
                setProjectActive(workspace->findProject(workspace->activeProject));
            }
        }
        else if (dat->file)
        {
            TextEditorPage* page = Manager::getInstancePtr()->getEditorPageManager()->openInTextEditor(dat->file->fname.GetFullPath());
            if (page)
                page->setProjectFile(dat->file);
        }
    }

    bool ProjectExplorer::loadWorkspace(const wxString& dir)
    {
        wxDir d(dir);
        if (!d.IsOpened())
            return false;

        workspace = new Workspace;

        // workspace form file system
        wxString dirName;
        bool dirOk = d.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
        while (dirOk)
        {
            String prjName = dirName.ToUTF8().data();
            Project::Ptr prj = new Project;
            prj->projectName = prjName;
            workspace->projects.push_back(prj);

            wxDir f(dir + dirName);
            wxString fileName;
            bool ok = f.GetFirst(&fileName, wxT("*.*"), wxDIR_FILES);
            while (ok)
            {
                ProjectFile::Ptr file = new ProjectFile;
                file->fileName = fileName.ToUTF8().data();
                prj->files.push_back(file);
                ok = f.GetNext(&fileName);
            }

            dirOk = d.GetNext(&dirName);
        }

        // the saved workspace, compared with the fs workspace
        Workspace::Ptr savedWorkspace = (Workspace*)Object::loadFromXml("/editor/workspace.xml");
        if (savedWorkspace)
        {
            Project::List::iterator it0 = savedWorkspace->projects.begin();
            for (; it0 != savedWorkspace->projects.end(); ++it0)
            {
                Project::Ptr savedPrj = *it0;
                Project::Ptr prj = workspace->findProject(savedPrj->projectName);
                if (prj)
                {
                    prj->debuggerName = savedPrj->debuggerName;
                    ProjectFile::List::iterator it1 = savedPrj->files.begin();
                    for (; it1 != savedPrj->files.end(); ++it1)
                    {
                        ProjectFile::Ptr savedFile = *it1;
                        ProjectFile::List::iterator it2 = prj->files.begin();
                        for (; it2 != prj->files.end(); ++it2)
                        {
                            if ((*it2)->fileName == savedFile->fileName)
                            {
                                *it2 = savedFile;
                                break;
                            }
                        }
                    }
                }
            }

            workspace->activeProject = savedWorkspace->activeProject;
            workspace->activePage = savedWorkspace->activePage;
        }

        typedef std::map<int, ProjectFile*> open_files_map;
        open_files_map open_files;

        Project::List::iterator it0 = workspace->projects.begin();
        for (; it0 != workspace->projects.end(); ++it0)
        {
            Project* prj = *it0;
            wxString projectName = wxString::FromUTF8(prj->projectName.c_str());
            ProjectFile::List::iterator it1 = prj->files.begin();
            wxTreeItemId treeId = projectTree->AppendItem(root, projectName, 1, 1);
            prj->treeNode = treeId;
            for (; it1 != prj->files.end(); ++it1)
            {
                wxString dirName = dir + projectName + wxT("/");
                wxString fileName = wxString::FromUTF8((*it1)->fileName.c_str());
                (*it1)->fname = wxFileName(dirName + fileName);
                (*it1)->fname.Normalize();
                (*it1)->treeId = projectTree->AppendItem(treeId, fileName, 2, 2, new TreeItemData(*it1));
                if ((*it1)->editorOpen)
                {
                    open_files[(*it1)->editorTabPos] = *it1;
                }
            }
        }

        open_files_map::iterator ite = open_files.begin();
        for (; ite != open_files.end(); ++ite)
        {
            TextEditorPage* page = Manager::getInstancePtr()->getEditorPageManager()->openInTextEditor(ite->second->fname.GetFullPath());
            if (page)
                page->setProjectFile(ite->second);
        }

        if (workspace->projects.size() > 0)
        {
            if (workspace->activeProject.empty())
                workspace->activeProject = (*workspace->projects.begin())->projectName;
            setProjectActive(workspace->findProject(workspace->activeProject));
        }

        projectTree->Expand(root);

        wxFlatNotebook* noteBook = Manager::getInstancePtr()->getEditorPageManager()->getNotebook();
        noteBook->SetSelection(workspace->activePage);

        return true;
    }

    void ProjectExplorer::saveWorkspace()
    {
        if (workspace)
        {
            updateProjectFiles();
            workspace->saveToXml("/editor/workspace.xml");
        }
    }

    void ProjectExplorer::updateProjectFiles()
    {
        Project::List::iterator it0 = workspace->projects.begin();
        for (; it0 != workspace->projects.end(); ++it0)
        {
            Project* prj = *it0;
            ProjectFile::List::iterator it1 = prj->files.begin();
            for (; it1 != prj->files.end(); ++it1)
            {
                ProjectFile* file = *it1;
                file->editorOpen = false;
            }
        }

        wxFlatNotebook* noteBook = Manager::getInstancePtr()->getEditorPageManager()->getNotebook();
        for (int i = 0; i < noteBook->GetPageCount(); ++i)
        {
            wxWindow* page = noteBook->GetPage(i);
            if (wxIsKindOf(page, TextEditorPage))
            {
                TextEditorPage* editor = static_cast<TextEditorPage*>(page);

                ProjectFile* prjFile = editor->getProjectFile();
                if (prjFile)
                {
                    prjFile->editorTopLine = editor->getControl()->GetFirstVisibleLine();
                    prjFile->editorPos = editor->getControl()->GetCurrentPos();
                    prjFile->editorTabPos = i + 1;
                    prjFile->editorOpen = true;
                }
            }
        }
        
        workspace->activePage = noteBook->GetSelection();
    }

    void ProjectExplorer::createImageList()
    {
        static const wxString imgs[] =
        {
            wxT("solution.png"),
            wxT("project.png"),
            wxT("file.png"),
        };

        wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
        imageList = new wxImageList(16, 16);

        wxBitmap bmp;
        for (int i = 0; i < sizeof(imgs) / sizeof(imgs[0]); ++i)
        {
            bmp.LoadFile(fullPath + imgs[i], wxBITMAP_TYPE_PNG);
            imageList->Add(bmp);
        }
    }

    void ProjectExplorer::setProjectActive(Project* project)
    {
        if (activeProject == project)
            return;

        if (activeProject)
            projectTree->SetItemBold(activeProject->treeNode, false);

        activeProject = project;

        projectTree->SetItemBold(activeProject->treeNode, true);
        projectTree->EnsureVisible(activeProject->treeNode);

        EditorEvent event(editorEVT_PROJECT_ACTIVED);
        event.project = activeProject;
        Manager::getInstancePtr()->processEvent(event);
    }

    Project* ProjectExplorer::getActiveProject()
    {
        return activeProject;
    }

    ProjectExplorer::~ProjectExplorer()
    {
        delete imageList;
        imageList = 0;
    }
}
