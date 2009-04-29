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
#include "WizardPlugin.h"
#include <wx/xrc/xmlres.h>
#include <wx/filename.h>
#include <wx/imaglist.h>

namespace hare
{
    int idTreeCtrlFilter = XRCID("treeCtrlFilter");
    int idListCtrlItems = XRCID("listCtrlItems");

    BEGIN_EVENT_TABLE(NewFileDialog, wxDialog)
        EVT_TREE_SEL_CHANGED(idTreeCtrlFilter, NewFileDialog::onTreeSelectChanged)
        EVT_LIST_ITEM_ACTIVATED(idListCtrlItems, NewFileDialog::onListActivated)
        EVT_LIST_ITEM_SELECTED(idListCtrlItems, NewFileDialog::onListSelected)
    END_EVENT_TABLE()

    class NFDTreeItemData : public wxTreeItemData
    {
    public:
        NFDTreeItemData(WizardPlugin* plugin) : wizard(plugin) {} 
        WizardPlugin* getPlugin() { return wizard; }
    private:
        WizardPlugin* wizard;
    };

    class NFDListItemData
    {
    public:
        NFDListItemData(WizardPlugin* plugin, int idx) : wizard(plugin), index(idx) {} 
        WizardPlugin* getPlugin() { return wizard; }
        int getIndex() { return index; }
    private:
        WizardPlugin* wizard;
        int index;
    };

    NewFileDialog::NewFileDialog(wxWindow* parent, const String& initial)
        : treeImageList(0), listImageList(0)
    {
        wxXmlResource::Get()->LoadDialog(this, parent, wxT("dlgNewFile"));

        treeCtrlFilter = XRCCTRL(*this, "treeCtrlFilter", wxTreeCtrl);
        listCtrlItems = XRCCTRL(*this, "listCtrlItems", wxListCtrl);
        textDesc = XRCCTRL(*this, "textDesc", wxStaticText);

        treeImageList = new wxImageList(16, 16);
        listImageList = new wxImageList(32, 32);

        wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
        wxBitmap bmp[2];
        bmp[0].LoadFile(fullPath + wxT("folder_closed.png"), wxBITMAP_TYPE_PNG);
        bmp[1].LoadFile(fullPath + wxT("folder.png"), wxBITMAP_TYPE_PNG);

        treeImageList->Add(bmp[0]);
        treeImageList->Add(bmp[1]);
        
        rebuildTree();
    }

    NewFileDialog::~NewFileDialog()
    {
        for (int i = 0; i < listCtrlItems->GetItemCount(); ++i)
        {
            NFDListItemData* data = (NFDListItemData*)listCtrlItems->GetItemData(i);
            delete data;
        }
        listCtrlItems->DeleteAllItems();

        delete treeImageList;
        delete listImageList;
        treeCtrlFilter->SetImageList(0);
        listCtrlItems->SetImageList(0, wxIMAGE_LIST_NORMAL);
    }

    void NewFileDialog::rebuildTree()
    {
        treeCtrlFilter->DeleteAllItems();
        listCtrlItems->DeleteAllItems();

        rootId = treeCtrlFilter->AddRoot(wxEmptyString);

        treeCtrlFilter->SetImageList(treeImageList);

        plugins = PluginManager::getInstancePtr()->findPlugins(EPT_Wizard);

        for (size_t i = 0; i < plugins.size(); ++i)
        {
            WizardPlugin* wizard = (WizardPlugin*)plugins[i];
            String folder = wizard->getFolder().ToUTF8().data();
            createTreeItem(folder, wizard);
        }
    }

    void NewFileDialog::rebuildList(WizardPlugin* plugin)
    {
        for (int i = 0; i < listCtrlItems->GetItemCount(); ++i)
        {
            NFDListItemData* data = (NFDListItemData*)listCtrlItems->GetItemData(i);
            delete data;
        }
        listCtrlItems->DeleteAllItems();
        
        listImageList->RemoveAll();

        for (int i = 0; i < plugin->getCount(); ++i)
        {
            int idx = listImageList->Add(plugin->getBitmap(i));

            int item = listCtrlItems->InsertItem(0, plugin->getTitle(i), idx);
            listCtrlItems->SetItemData(item, (long)new NFDListItemData(plugin, i));
        }

        listCtrlItems->SetImageList(listImageList, wxIMAGE_LIST_NORMAL);
    }

    void NewFileDialog::onTreeSelectChanged(wxTreeEvent& event)
    {
        wxTreeItemId id = event.GetItem();
        NFDTreeItemData* itemData = (NFDTreeItemData*)treeCtrlFilter->GetItemData(id);
        if (itemData)
        {
            WizardPlugin* wizard = itemData->getPlugin();
            rebuildList(wizard);
        }
    }

    bool NewFileDialog::createTreeItem(const String& folder, WizardPlugin* plugin)
    {
        StringVector dirs = StringUtil::split(folder, "/");

        wxTreeItemId nextId = rootId;

        for (size_t i = 0; i < dirs.size(); ++i)
        {
            bool itemExists = false;
            wxTreeItemIdValue cookie;
            wxTreeItemId childId = treeCtrlFilter->GetFirstChild(nextId, cookie);
            wxString text = wxString::FromUTF8(dirs[i].c_str());
            while (childId.IsOk())
            {
                if (text == treeCtrlFilter->GetItemText(childId))
                {
                    itemExists = true;
                    nextId = childId;
                    break;
                }
                childId = treeCtrlFilter->GetNextChild(nextId, cookie);
            }

            if (itemExists)
                continue;

            nextId = treeCtrlFilter->AppendItem(nextId, text, -1, -1);
            treeCtrlFilter->SetItemImage(nextId, 0, wxTreeItemIcon_Normal);
            treeCtrlFilter->SetItemImage(nextId, 1, wxTreeItemIcon_Selected);
            treeCtrlFilter->SetItemImage(nextId, 0, wxTreeItemIcon_Expanded);
            treeCtrlFilter->SetItemImage(nextId, 1, wxTreeItemIcon_SelectedExpanded);
        }

        treeCtrlFilter->SetItemData(nextId, new NFDTreeItemData(plugin));

        return true;
    }

    void NewFileDialog::onListActivated(wxListEvent& event)
    {
        NFDListItemData* data = (NFDListItemData*)event.GetData();
        data->getPlugin()->wizard(data->getIndex());
    }

    void NewFileDialog::onListSelected(wxListEvent& event)
    {
        NFDListItemData* data = (NFDListItemData*)event.GetData();
        wxString desc = data->getPlugin()->getDesc(data->getIndex());
        textDesc->SetLabel(desc);
    }
}
