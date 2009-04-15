//***************************************************************
//  File:    NewFileDialog.h
//  Data:    01/26/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include "EditorPrerequisites.h"
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/dialog.h>

namespace hare
{
    class EDITOR_API NewFileDialog : public wxDialog
    {
    public:
        NewFileDialog(wxWindow* parent, wxString initial = wxEmptyString);
        virtual ~NewFileDialog();

        bool createFolder(const wxString& folder);
        bool selectFolder(const wxString& folder, bool expand = true);

    protected:
        //void onCategoryChanged(wxCommandEvent& event);
        //void onUpdateUI(wxUpdateUIEvent& event);

    private:
        wxListCtrl* listCtrlItems;
        wxTreeCtrl* treeCtrlFilter;
        wxTreeItemId rootId;

        DECLARE_EVENT_TABLE()
    };

}
#endif

