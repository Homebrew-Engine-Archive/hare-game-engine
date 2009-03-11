//***************************************************************
//  File:    NewFileDialog.cpp
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
#include "PCH.h"
#include "NewFileDialog.h"
#include "EditorManager.h"
#include "ConfigManager.h"
#include <wx/xrc/xmlres.h>
#include <wx/filename.h>
#include <wx/imaglist.h>

namespace hare_editor
{
    BEGIN_EVENT_TABLE(NewFileDialog, wxDialog)
    END_EVENT_TABLE()

    NewFileDialog::NewFileDialog(wxWindow* parent, wxString initial)
    {
        wxXmlResource::Get()->LoadDialog(this, parent, wxT("dlgNewFile"));

        treeCtrlFilter = XRCCTRL(*this, "treeCtrlFilter", wxTreeCtrl);
        listCtrlItems = XRCCTRL(*this, "listCtrlItems", wxListCtrl);

        rootId = treeCtrlFilter->AddRoot(wxEmptyString);

        wxBitmap bmp[2];

        wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/resources/");
        wxImageList* imgList = new wxImageList(16, 16);

        bmp[0].LoadFile(fullPath + wxT("folder_closed.png"), wxBITMAP_TYPE_PNG);
        bmp[1].LoadFile(fullPath + wxT("folder.png"), wxBITMAP_TYPE_PNG);

        imgList->Add(bmp[0]);
        imgList->Add(bmp[1]);

        treeCtrlFilter->SetImageList(imgList);

        selectFolder(initial);

        Center();
    }

    NewFileDialog::~NewFileDialog()
    {
        delete treeCtrlFilter->GetImageList();
        treeCtrlFilter->SetImageList(0);
    }

    bool NewFileDialog::selectFolder(const wxString& folder, bool expand)
    {
        if (folder.IsEmpty())
            return true;

        wxFileName fname(folder);
        wxArrayString dirs = fname.GetDirs();

        wxTreeItemId nextId = rootId;

        for (size_t i = 0; i < dirs.GetCount(); ++i)
        {
            bool itemExists = false;
            wxTreeItemIdValue cookie;
            wxTreeItemId childId = treeCtrlFilter->GetFirstChild(nextId, cookie);
            while (childId.IsOk())
            {
                if (dirs[i] == treeCtrlFilter->GetItemText(childId))
                {
                    itemExists = true;
                    nextId = childId;
                    break;
                }
                childId = treeCtrlFilter->GetNextChild(nextId, cookie);
            }

            if (itemExists)
                continue;
            else
                return false;
        }

        treeCtrlFilter->SelectItem(nextId, true);

        if (expand)
            treeCtrlFilter->Expand(nextId);

        return true;
    }

    bool NewFileDialog::createFolder(const wxString& folder)
    {
        wxFileName fname(folder);
        wxArrayString dirs = fname.GetDirs();

        wxTreeItemId nextId = rootId;

        for (size_t i = 0; i < dirs.GetCount(); ++i)
        {
            bool itemExists = false;
            wxTreeItemIdValue cookie;
            wxTreeItemId childId = treeCtrlFilter->GetFirstChild(nextId, cookie);
            while (childId.IsOk())
            {
                if (dirs[i] == treeCtrlFilter->GetItemText(childId))
                {
                    itemExists = true;
                    nextId = childId;
                    break;
                }
                childId = treeCtrlFilter->GetNextChild(nextId, cookie);
            }

            if (itemExists)
                continue;

            nextId = treeCtrlFilter->AppendItem(nextId, dirs[i], -1, -1);
            treeCtrlFilter->SetItemImage(nextId, 0, wxTreeItemIcon_Normal);
            treeCtrlFilter->SetItemImage(nextId, 1, wxTreeItemIcon_Selected);
            treeCtrlFilter->SetItemImage(nextId, 0, wxTreeItemIcon_Expanded);
            treeCtrlFilter->SetItemImage(nextId, 1, wxTreeItemIcon_SelectedExpanded);
        }

        return true;
    }
}
