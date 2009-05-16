//***************************************************************
//  File:    FileSystemExplorer.h
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
#ifndef FILESYSTEM_EXPLORER_H
#define FILESYSTEM_EXPLORER_H

#include "EditorPrerequisites.h"
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/imaglist.h>
#include <wx/dialog.h>

namespace hare
{
    //---------------------------------------------------------------
    // FileSystemPanel : the common panel showing the file system
    //---------------------------------------------------------------
    class FileSystemPanel : public wxPanel
    {
    public:
        FileSystemPanel(wxWindow *parent);
       ~FileSystemPanel();

        void update(const wxString& filePath);
        void setFilter(const wxString& fileExt);
        void refresh();

        wxString getUrl() const
        {
            return curUrl;
        }

    private:
        void onGoUp(wxCommandEvent& event);
        void onRefresh(wxCommandEvent& event);
        void onChangeView(wxCommandEvent& event);
        void onChangeViewMenu(wxCommandEvent& event);

        void recreateList(long flags);

    protected:
        wxListCtrl* listCtrl;
        wxStaticText* url;
        wxImageList* smallList;
        wxString filter;

        wxString curUrl;

        DECLARE_EVENT_TABLE()
    };

    //---------------------------------------------------------------
    // FileSystemExplorer : shown in the Explorer
    //---------------------------------------------------------------
    class FileSystemExplorer : public FileSystemPanel
    {
    public:
        FileSystemExplorer(wxWindow *parent);

    private:
        void onDoubleClicked(wxListEvent& event);
    };

    //---------------------------------------------------------------
    // FileSystemDialog : this is a dialog for selecting files
    //---------------------------------------------------------------
    class EDITOR_API FileSystemDialog : public wxDialog
    {
    public:
        FileSystemDialog(wxWindow *parent, bool addStdDialogButton = false,
            const wxString& message = wxT("FileSystemDialog"),
            const wxString& defaultPath = wxEmptyString,
            long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            const wxString& name = wxT("FileSystemDialog"));

        wxString GetPath() const;
        wxString GetDir() const;
        void SetPath(const wxString& path);
        void SetFilter(const wxString& fileExt);

    private:
        FileSystemPanel* panel;
        wxString file;

    private:
        void onDoubleClicked(wxListEvent& event);
    };
}

#endif
