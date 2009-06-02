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
#include "EditorPluginManager.h"
#include "WizardPlugin.h"
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/dialog.h>

namespace hare
{
    class NFDListItemData;

    class EDITOR_API NewFileDialog : public wxDialog
    {
    public:
        NewFileDialog(wxWindow* parent, const String& initial = StringUtil::EMPTY);
        virtual ~NewFileDialog();

        void rebuildTree();

    protected:
        void onTreeSelectChanged(wxTreeEvent& event);
        void onListActivated(wxListEvent& event);
        void onListSelected(wxListEvent& event);
        
        void rebuildList(WizardPlugin* plugin);
        bool createTreeItem(const String& folder, WizardPlugin* plugin);

        void onOK(wxCommandEvent& event);

    private:
        wxListCtrl* listCtrlItems;
        wxTreeCtrl* treeCtrlFilter;
        wxStaticText* textDesc;

        wxImageList* treeImageList;
        wxImageList* listImageList;

        NFDListItemData* currItemData;

        wxTreeItemId rootId;

        PluginsArray plugins;

        DECLARE_EVENT_TABLE()
    };

}
#endif

