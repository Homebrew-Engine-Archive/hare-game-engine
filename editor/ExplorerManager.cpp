//***************************************************************
//  File:    ExplorerManager.cpp
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
#include "ExplorerManager.h"
#include "EditorManager.h"
#include "ConfigManager.h"
#include "EditorPageManager.h"
#include "PropertyHandler.h"
#include "FileSystemExplorer.h"
#include "ProjectExplorer.h"
#include <wx/utils.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#include <wx/imaglist.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/xrc/xmlres.h>

namespace hare
{
    template<> ExplorerManager* TManager<ExplorerManager>::instance = 0;
    template<> bool TManager<ExplorerManager>::autoCreate = true;

    static const int idExplorerManagerNoteBook = wxNewId();
    static const int idFileSystemExplorer = XRCID("panelFSExplorer");
    static const int idPropertyGridManager = wxNewId();

    ExplorerManager::ExplorerManager()
        : notebook(0), propGridManager(0), fsExplorer(0)
    {
        notebook = new wxFlatNotebook(Manager::getInstancePtr()->getAppWindow(), idExplorerManagerNoteBook);

        notebook->SetWindowStyleFlag(wxFNB_BOTTOM | wxFNB_NO_X_BUTTON);

        createProjectPage();
        createFileSystemPage();
        createPropertyPage();
    }

    void ExplorerManager::createFileSystemPage()
    {
        fsExplorer = new FileSystemExplorer(notebook);

        notebook->AddPage(fsExplorer, _("FileSystem"));
    }

    void ExplorerManager::createPropertyPage()
    {
        propGridManager = new wxPropertyGridManager(notebook,
            idPropertyGridManager, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED |
            wxPG_SPLITTER_AUTO_CENTER | wxPG_TOOLBAR | wxPG_DESCRIPTION);

        propGridManager->SetExtraStyle(wxPG_EX_MODE_BUTTONS | wxPG_EX_UNFOCUS_ON_ENTER | wxPG_EX_HELP_AS_TOOLTIPS);
        propGridManager->SetValidationFailureBehavior(wxPG_VFB_BEEP | wxPG_VFB_MARK_CELL | wxPG_VFB_SHOW_MESSAGE);
        propGridManager->GetGrid()->SetVerticalSpacing(2);

        notebook->AddPage(propGridManager, _("Properties"));
    }

    void ExplorerManager::createProjectPage()
    {
        projectExplorer = new ProjectExplorer(notebook);

        notebook->AddPage(projectExplorer, _("Project"));
    }

    ExplorerManager::~ExplorerManager()
    {
        // Must destroy this notebook before unload plugins, because
        // PropGridManager's page may hold the ref to an object which
        // should be released before unload the renderer plugin.
        // propGridManager->Clear() doesn't work :( , for it do not 
        // delete the page.
        notebook->Destroy();
    }

    void ExplorerManager::bindProperty(const wxString& name, Object* object, EditorPage* associatedPage)
    {
        PropertyGridPage* page = new PropertyGridPage(associatedPage);

        propGridManager->AddPage(name, wxPG_NULL_BITMAP, page);

        setPropertyGridObject(page, object);

        propGridManager->Refresh();
    }

    void ExplorerManager::removeAllProperties()
    {
        if (!Manager::isAppShuttingDown())
        {
            propGridManager->Clear();
            propGridManager->Refresh();
        }
    }
}
