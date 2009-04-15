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
#include "PluginManager.h"
#include "MIMEHandlerPlugin.h"
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>
#include <wx/imaglist.h>
#include <wx/menu.h>

namespace hare
{
    // ---------------------------------------------------------------
    // FileSystemPanel
    // ---------------------------------------------------------------
    int idFSPanelUp = XRCID("idFSPanelUp");
    int idFSPanelRefresh = XRCID("idFSPanelRefresh");
    int idFSPanelURL = XRCID("idFSPanelURL");
    int idFSPanelChageView = XRCID("idFSPanelChageView");
    int idFSPanelList = wxNewId();
    int idFSPanelIconView = wxNewId();
    int idFSPanelListView = wxNewId();
    int idFSPanelReportView = wxNewId();

    BEGIN_EVENT_TABLE(FileSystemPanel, wxPanel)
        EVT_MENU(idFSPanelUp, FileSystemPanel::onGoUp)
        EVT_MENU(idFSPanelRefresh, FileSystemPanel::onRefresh)
        EVT_MENU(idFSPanelChageView, FileSystemPanel::onChangeView)
        EVT_MENU_RANGE(idFSPanelIconView, idFSPanelReportView, FileSystemPanel::onChangeViewMenu)
    END_EVENT_TABLE()

    FileSystemPanel::FileSystemPanel(wxWindow *parent)
        : listCtrl(0)
    {
        wxXmlResource::Get()->LoadPanel(this, parent, wxT("panelFSPanel"));
        url = XRCCTRL(*this, "idFSPanelURL", wxStaticText);

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

    FileSystemPanel::~FileSystemPanel()
    {
        delete smallList;
        listCtrl->SetImageList(0, wxIMAGE_LIST_SMALL);
    }

    void FileSystemPanel::recreateList(long flags)
    {
        delete listCtrl;

        listCtrl = new wxListCtrl(this, idFSPanelList, wxDefaultPosition, wxDefaultSize,
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

    void FileSystemPanel::onGoUp(wxCommandEvent& event)
    {
        if (curUrl == wxT("/"))
            return;

        wxString path = curUrl.Mid(0, curUrl.Len() - 1);
        int lastSep = path.Find(wxT('/'), true);
        path = path.Mid(0, lastSep);
        update(path);
    }

    void FileSystemPanel::onRefresh(wxCommandEvent& event)
    {
        refresh();
    }

    void FileSystemPanel::refresh()
    {
        update(curUrl);
    }

    void FileSystemPanel::onChangeView(wxCommandEvent& event)
    {
        wxMenu menuPopup;

        menuPopup.AppendRadioItem(idFSPanelReportView, _("&Report View"));
        menuPopup.AppendRadioItem(idFSPanelListView, _("&List View"));

        int checkId = -1;

        switch (listCtrl->GetWindowStyleFlag() & wxLC_MASK_TYPE)
        {
        case wxLC_LIST:
            checkId = idFSPanelListView;
            break;
        case wxLC_REPORT:
            checkId = idFSPanelReportView;
            break;
        }

        menuPopup.Check(checkId, true);

        PopupMenu(&menuPopup);
    }

    void FileSystemPanel::onChangeViewMenu(wxCommandEvent& event)
    {
        int id = event.GetId();
        if (id == idFSPanelListView)
            recreateList(wxLC_LIST);
        else if (id == idFSPanelReportView)
            recreateList(wxLC_REPORT);
    }

    void FileSystemPanel::update(const wxString& path)
    {
        curUrl = path;

        if (!curUrl.StartsWith(wxT("/")))
            curUrl = wxT("/") + curUrl;

        String pathUTF8 = curUrl.ToUTF8().data();

        FileSystem* fs = FileSystem::getSingletonPtr();

        while (!fs->isDir(pathUTF8))
        {
            curUrl = curUrl.Mid(0, curUrl.Len() - 1);
            int lastSep = path.Find(wxT('/'), true);
            curUrl = curUrl.Mid(0, lastSep);
            pathUTF8 = curUrl.ToUTF8().data();
        }

        if (!curUrl.EndsWith(wxT("/")))
            curUrl += wxT("/");

        pathUTF8 = curUrl.ToUTF8().data();

        url->SetLabel(curUrl);

        listCtrl->DeleteAllItems();

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

        if ((listCtrl->GetWindowStyleFlag() & wxLC_REPORT) &&
            listCtrl->GetItemCount() > 0)
        {
            listCtrl->SetColumnWidth(0, wxLIST_AUTOSIZE);
            listCtrl->SetColumnWidth(1, wxLIST_AUTOSIZE);
        }
    }

    // ---------------------------------------------------------------
    // FileSystemExplorer
    // ---------------------------------------------------------------
    FileSystemExplorer::FileSystemExplorer(wxWindow *parent)
     : FileSystemPanel(parent)
    {
        Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            wxListEventHandler(FileSystemExplorer::onDoubleClicked), 0, this);
    }

    void FileSystemExplorer::onDoubleClicked(wxListEvent& event)
    {
        wxString path = curUrl + event.GetText();
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
                // MIME Handler
                PluginsArray plugins = Manager::getInstancePtr()->getPluginManager()->findPlugins(EPT_MIMEHandler);
                for (size_t i = 0; i < plugins.size(); ++i)
                {
                    MIMEHandlerPlugin* plugin = (MIMEHandlerPlugin*)plugins[i];
                    if (plugin->canHandle(path))
                    {
                        plugin->openFile(path);
                        break;
                    }
                }
            }
        }
    }

    // ---------------------------------------------------------------
    // FileSystemExplorer
    // ---------------------------------------------------------------
    FileSystemDialog::FileSystemDialog(wxWindow *parent,
        const wxString& message, const wxString& defaultPath, long style,
        const wxPoint& pos, const wxSize& size, const wxString& name)
     : wxDialog(parent, wxID_ANY, message, pos, size, style, name)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        panel = new FileSystemPanel(this);
        sizer->Add(panel, 1, wxALIGN_LEFT | wxALIGN_TOP | wxEXPAND, 0);
        SetSizer(sizer);
        Layout();

        panel->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            wxListEventHandler(FileSystemDialog::onDoubleClicked), 0, this);

        SetPath(defaultPath);
    }

    wxString FileSystemDialog::GetPath() const
    {
        return panel->getUrl() + file;
    }

    void FileSystemDialog::SetPath(const wxString& path)
    {
        panel->update(path);
    }

    void FileSystemDialog::onDoubleClicked(wxListEvent& event)
    {
        wxString path = panel->getUrl() + event.GetText();
        String pathUTF8 = path.ToUTF8().data();

        FileSystem* fs = FileSystem::getSingletonPtr();
        if (fs->exists(pathUTF8))
        {
            if (fs->isDir(pathUTF8))
            {
                panel->update(wxString::FromUTF8(pathUTF8.c_str()));
            }
            else
            {
                file = event.GetText();
                EndModal(wxID_OK);
            }
        }
    }
}
