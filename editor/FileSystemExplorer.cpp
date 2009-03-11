//***************************************************************
//  File:    FileSystemExplorer.cpp
//  Data:    01/29/2009
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
#include "FileSystemExplorer.h"
#include "EditorManager.h"
#include "EditorPageManager.h"
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>
#include <wx/imaglist.h>

namespace hare_editor
{
    int idFSExplorerList = wxNewId();
    int idFSExplorerUp = XRCID("idFSExplorerUp");
    int idFSExplorerRefresh = XRCID("idFSExplorerRefresh");
    int idFSExplorerURL = XRCID("idFSExplorerURL");
    int idFSExplorerChageView = XRCID("idFSExplorerChageView");
    int idFSEIconView = wxNewId();
    int idFSEListView = wxNewId();
    int idFSEReportView = wxNewId();

    BEGIN_EVENT_TABLE(FileSystemExplorer, wxPanel)
        EVT_MENU(idFSExplorerUp, FileSystemExplorer::onGoUp)
        EVT_MENU(idFSExplorerRefresh, FileSystemExplorer::onRefresh)
        EVT_MENU(idFSExplorerChageView, FileSystemExplorer::onChangeView)
        EVT_MENU_RANGE(idFSEIconView, idFSEReportView, FileSystemExplorer::onChangeViewMenu)
        EVT_LIST_ITEM_ACTIVATED(idFSExplorerList, FileSystemExplorer::onDoubleClicked)
    END_EVENT_TABLE()

    FileSystemExplorer::FileSystemExplorer(wxWindow *parent)
        : listCtrl(0)
    {
        wxXmlResource::Get()->LoadPanel(this, parent, wxT("panelFSExplorer"));
        url = XRCCTRL(*this, "idFSExplorerURL", wxStaticText);

        smallList = new wxImageList(16, 16);

        wxBitmap bmp[4];
        wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/resources/");
        bmp[0].LoadFile(fullPath + wxT("folder.png"), wxBITMAP_TYPE_PNG);
        bmp[1].LoadFile(fullPath + wxT("file.png"), wxBITMAP_TYPE_PNG);
        smallList->Add(bmp[0]);
        smallList->Add(bmp[1]);

        recreateList(wxLC_REPORT);

        update(wxT("/"));
    }

    FileSystemExplorer::~FileSystemExplorer()
    {
        delete smallList;
        listCtrl->SetImageList(0, wxIMAGE_LIST_SMALL);
    }

    void FileSystemExplorer::recreateList(long flags)
    {
        delete listCtrl;

        listCtrl = new wxListCtrl(this, idFSExplorerList, wxDefaultPosition, wxDefaultSize,
            flags | wxLC_SORT_ASCENDING | wxLC_AUTOARRANGE);

        if ((flags & wxLC_MASK_TYPE) == wxLC_REPORT)
        {
            listCtrl->InsertColumn(0, _("Name"));
            listCtrl->InsertColumn(1, _("Real Directory"));
        }

        listCtrl->SetImageList(smallList, wxIMAGE_LIST_SMALL);

        GetSizer()->Add(listCtrl, 1, wxEXPAND | wxALL);
        Layout();

        refresh();
    }

    void FileSystemExplorer::onGoUp(wxCommandEvent& event)
    {
        if (curPath == wxT("/"))
            return;

        wxString path = curPath.Mid(0, curPath.Len() - 1);
        int lastSep = path.Find(wxT('/'), true);
        path = path.Mid(0, lastSep);
        update(path);
    }

    void FileSystemExplorer::onRefresh(wxCommandEvent& event)
    {
        refresh();
    }

    void FileSystemExplorer::refresh()
    {
        update(curPath);
    }

    void FileSystemExplorer::onDoubleClicked(wxListEvent& event)
    {
        wxString path = curPath + event.GetText();
        String pathUTF8 = path.ToUTF8().data();

        FileSystem* fs = FileSystem::getSingletonPtr();
        if (fs->exists(pathUTF8))
        {
            if (fs->isDir(pathUTF8))
            {
                update(wxString::FromUTF8(pathUTF8.c_str()));
            }
            else
            {
                const char* realDir = fs->getRealDir(pathUTF8);
                wxFileName fname(wxString::FromUTF8(realDir) + curPath, event.GetText());
                Manager::getInstancePtr()->getEditorPageManager()->openInTextEditor(fname.GetFullPath());
            }
        }
    }

    void FileSystemExplorer::onChangeView(wxCommandEvent& event)
    {
        wxMenu menuPopup;

        menuPopup.AppendRadioItem(idFSEReportView, _("&Report View"));
        menuPopup.AppendRadioItem(idFSEListView, _("&List View"));

        int checkId = -1;

        switch (listCtrl->GetWindowStyleFlag() & wxLC_MASK_TYPE)
        {
        case wxLC_LIST:
            checkId = idFSEListView;
            break;
        case wxLC_REPORT:
            checkId = idFSEReportView;
            break;
        }

        menuPopup.Check(checkId, true);

        PopupMenu(&menuPopup);
    }

    void FileSystemExplorer::onChangeViewMenu(wxCommandEvent& event)
    {
        int id = event.GetId();
        if (id == idFSEListView)
            recreateList(wxLC_LIST);
        else if (id == idFSEReportView)
            recreateList(wxLC_REPORT);
    }

    void FileSystemExplorer::update(const wxString& path)
    {
        curPath = path;

        if (!curPath.EndsWith(wxT("/")))
            curPath += wxT("/");

        url->SetLabel(curPath);

        listCtrl->DeleteAllItems();

        String pathUTF8 = curPath.ToUTF8().data();

        FileSystem* fs = FileSystem::getSingletonPtr();
        StringVector files = fs->enumFiles(pathUTF8);
        int nextPos = 0;

        for (size_t i = 0; i < files.size(); ++i)
        {
            String fullFileName = pathUTF8 + files[i];
            int icoIndex = fs->isDir(fullFileName) ? 0 : 1;
            nextPos = listCtrl->InsertItem(nextPos, wxString::FromUTF8(files[i].c_str()), icoIndex);
            if (listCtrl->GetWindowStyleFlag() & wxLC_REPORT)
            {
                const char* realDir = fs->getRealDir(fullFileName);
                listCtrl->SetItem(nextPos, 1, wxString::FromUTF8(realDir));
            }
        }

        if (listCtrl->GetWindowStyleFlag() & wxLC_REPORT && listCtrl->GetItemCount() > 0)
        {
            listCtrl->SetColumnWidth(0, wxLIST_AUTOSIZE);
            listCtrl->SetColumnWidth(1, wxLIST_AUTOSIZE);
        }
    }
}
