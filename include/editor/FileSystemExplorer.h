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

namespace hare_editor
{
    class FileSystemExplorer : public wxPanel
    {
    public:
        FileSystemExplorer(wxWindow *parent);
        ~FileSystemExplorer();

        void update(const wxString& path);
        void refresh();

    private:
        void onGoUp(wxCommandEvent& event);
        void onRefresh(wxCommandEvent& event);
        void onDoubleClicked(wxListEvent& event);
        void onChangeView(wxCommandEvent& event);
        void onChangeViewMenu(wxCommandEvent& event);

        void recreateList(long flags);

    private:
        wxListCtrl* listCtrl;
        wxStaticText* url;
        wxString curPath;
        wxImageList* smallList;

        DECLARE_EVENT_TABLE()
    };
}

#endif