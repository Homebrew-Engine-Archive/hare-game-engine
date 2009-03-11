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

namespace hare_editor
{
    ProjectFile* ScriptPackage::findFile(const String& name)
    {
        ProjectFile::List::iterator it = files.begin();
        for (; it != files.end(); ++it)
        {
            if ((*it)->fileName == name)
                return *it;
        }

        return 0;
    }

    ScriptPackage* Project::findPackage(const String& name)
    {
        ScriptPackage::List::iterator it = packages.begin();
        for (; it != packages.end(); ++it)
        {
            if ((*it)->packageName == name)
                return *it;
        }

        return 0;
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(ScriptPackage, Object, 0)
    {
        HARE_META(packageName, String)
        HARE_OBJ_LIST(files, ProjectFile)
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(Project, Object, 0)
    {
        HARE_OBJ_LIST(packages, ScriptPackage)
    }

    int idProjectExplorer = wxNewId();
    
    BEGIN_EVENT_TABLE(ProjectExplorer, wxPanel)
        EVT_TREE_ITEM_ACTIVATED(idProjectExplorer, ProjectExplorer::onProjectFileActivated)
    END_EVENT_TABLE()

    ProjectExplorer::ProjectExplorer(wxWindow *parent)
        : wxPanel(parent), imageList(0)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        projectTree = new wxTreeCtrl(this, idProjectExplorer, wxDefaultPosition, wxDefaultSize,
            wxTR_EDIT_LABELS | wxTR_HAS_BUTTONS | wxNO_BORDER);

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

        if (dat && dat->file)
        {
            TextEditorPage* page = Manager::getInstancePtr()->getEditorPageManager()->openInTextEditor(dat->file->fname.GetFullPath());
            if (page)
                page->setProjectFile(dat->file);
        }
    }

    bool ProjectExplorer::loadProject(const wxString& dir)
    {
        wxDir d(dir);
        if (!d.IsOpened())
            return false;

        project = new Project;

        wxString dirName;
        bool dirOk = d.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
        while (dirOk)
        {
            String pkgName = dirName.ToUTF8().data();
            ScriptPackage* pkg = new ScriptPackage;
            pkg->packageName = pkgName;
            project->packages.push_back(pkg);

            wxDir f(dir + dirName);
            wxString fileName;
            bool ok = f.GetFirst(&fileName, wxT("*.*"), wxDIR_FILES);
            while (ok)
            {
                ProjectFile* file = new ProjectFile;
                file->fileName = fileName.ToUTF8().data();
                pkg->files.push_back(file);
                ok = f.GetNext(&fileName);
            }

            dirOk = d.GetNext(&dirName);
        }

        Project::Ptr savedProject = (Project*)Object::loadFromXml("project.xml");
        if (savedProject)
        {
            ScriptPackage::List::iterator it0 = savedProject->packages.begin();
            for (; it0 != savedProject->packages.end(); ++it0)
            {
                ScriptPackage* savedPackage = *it0;
                ScriptPackage* package = project->findPackage(savedPackage->packageName);
                if (package)
                {
                    ProjectFile::List::iterator it1 = savedPackage->files.begin();
                    for (; it1 != savedPackage->files.end(); ++it1)
                    {
                        ProjectFile* savedFile = *it1;
                        ProjectFile* file = package->findFile(savedFile->fileName);
                        if (file)
                            *file = *savedFile;
                    }
                }
            }
        }

        typedef std::map<int, ProjectFile*> open_files_map;
        open_files_map open_files;

        ScriptPackage::List::iterator it0 = project->packages.begin();
        for (; it0 != project->packages.end(); ++it0)
        {
            ScriptPackage* pkg = *it0;
            wxString packageName = wxString::FromUTF8(pkg->packageName.c_str());
            ProjectFile::List::iterator it1 = pkg->files.begin();
            wxTreeItemId treeId = projectTree->AppendItem(root, packageName, 1, 1);
            for (; it1 != pkg->files.end(); ++it1)
            {
                wxString dirName = dir + packageName + wxT("/");
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

        //wxFlatNotebook* noteBook = Manager::getInstancePtr()->getEditorPageManager()->getNotebook();
        //noteBook->SetSelection(project->activePage);

        projectTree->Expand(root);

        return true;
    }

    void ProjectExplorer::saveProject()
    {
        if (project)
        {
            updateProjectFiles();
            project->saveToXml("project.xml");
        }
    }

    void ProjectExplorer::updateProjectFiles()
    {
        //ProjectFile::List& list = project->files;
        //ProjectFile::List::iterator it = list.begin();
        //for (; it != list.end(); ++it)
        //{
        //    (*it)->editorOpen = false;
        //}

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
        //project->activePage = noteBook->GetSelection();
    }

    void ProjectExplorer::createImageList()
    {
        static const wxString imgs[] =
        {
            wxT("solution.png"),
            wxT("project.png"),
            wxT("file.png"),
        };

        wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/resources/");
        imageList = new wxImageList(16, 16);

        wxBitmap bmp;
        for (int i = 0; i < sizeof(imgs) / sizeof(imgs[0]); ++i)
        {
            bmp.LoadFile(fullPath + imgs[i], wxBITMAP_TYPE_PNG);
            imageList->Add(bmp);
        }
    }


    ProjectExplorer::~ProjectExplorer()
    {
        delete imageList;
        imageList = 0;
    }
}
