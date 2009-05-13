//***************************************************************
//  File:    UVRCMIMEHandler.cpp
//  Data:    03/20/2009
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
#include "UVRCMIMEHandler.h"
#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>


class NameDialog : public wxDialog
{
public:
    NameDialog::NameDialog(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString) 
            : wxDialog(parent, id, title, wxDefaultPosition, wxSize(280, 115))
    {
        this->SetSizeHints(wxDefaultSize, wxDefaultSize);

        wxBoxSizer* sizer;
        sizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* staticText = new wxStaticText(this, wxID_ANY, wxT("Name :"), wxDefaultPosition, wxDefaultSize, 0);
        staticText->Wrap(-1);
        sizer->Add(staticText, 0, wxALL, 5);

        txtName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
        sizer->Add(txtName, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5);

        wxStdDialogButtonSizer* sdbSizer = new wxStdDialogButtonSizer();
        sdbSizer->AddButton(new wxButton(this, wxID_OK));
        sdbSizer->AddButton(new wxButton(this, wxID_CANCEL));
        sdbSizer->Realize();
        sizer->Add(sdbSizer, 0, wxALIGN_RIGHT | wxEXPAND, 5);

        this->SetSizer(sizer);
        this->Layout();
        
    }
    void setText(const wxString& text)
    {
        txtName->SetValue(text);
    }
    wxString getText() const
    {
        return txtName->GetValue();
    }
private:
    wxTextCtrl* txtName;
};

IMPLEMENT_ABSTRACT_CLASS(UVRCEditorPage, UVEditorPage)

int idMenuAddItem = wxNewId();
int idMenuRenameItem = wxNewId();
int idMenuDelItem = wxNewId();

BEGIN_EVENT_TABLE(UVRCEditorPage, UVEditorPage)
    EVT_MENU(idMenuAddItem, UVRCEditorPage::onMenuEvent)
    EVT_MENU(idMenuRenameItem, UVRCEditorPage::onMenuEvent)
    EVT_MENU(idMenuDelItem, UVRCEditorPage::onMenuEvent)
END_EVENT_TABLE()

UVRCEditorPage::UVRCEditorPage(wxWindow* parent, UVRCMIMEHandler* handler)
 : UVEditorPage(parent), mime(handler), isModified(false)
{
    list->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(UVRCEditorPage::onRightClick), 0, this);

    updateTitle();
}

UVRCEditorPage::~UVRCEditorPage()
{
    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
}

void UVRCEditorPage::updateTitle()
{
    String title = "[UVRCEditor]";
    if (state) title += state->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}

void UVRCEditorPage::setModified(bool modified)
{
    if (modified != isModified)
    {
        isModified = modified;
        updateTitle();
    }
}

bool UVRCEditorPage::saveAs()
{
    bool ret = false;

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        _T("Save uv resource as"),
        _T(""),
        _T(""),
        _T("UV Resource (*.uvrc)|*.uvrc|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK)
    {
        static const char* tempFileName = "/editor/~uvrc.temp";
        FileSystem::getSingletonPtr()->remove(tempFileName);
        state->saveToXml(tempFileName);
        String dir = FileSystem::getSingletonPtr()->getRealDir(tempFileName);
        dir += tempFileName;
        wxString tempFile = wxString::FromUTF8(dir.c_str());
        wxFileName fName(tempFile);
        if (fName.FileExists())
        {
            wxString fileName = dlg->GetPath();
            ret = wxCopyFile(fName.GetFullPath(), fileName);
        }
    }

    dlg->Destroy();
    return ret;
}

bool UVRCEditorPage::save()
{
    if (!state)
        return true;

    bool ret = false;

    if (state->getUrl().empty())
        ret = saveAs();
    else
        ret = state->saveToXml(state->getUrl());

    if (ret)
        setModified(false);

    return ret;
}

void UVRCEditorPage::setUVState(UVEditorState* state)
{
    UVEditorPage::setUVState(state);
    updateTitle();
}

bool UVRCEditorPage::Show(bool show)
{
    if (show)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        if (state)
            Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("UVRCEditorProperity"), state);
    }
    return EditorPage::Show(show);
}

void UVRCEditorPage::onMenuEvent(wxCommandEvent& event)
{
    if (event.GetId() == idMenuAddItem)
    {
        NameDialog* dlg = new NameDialog(this, wxID_ANY, _("Add"));
        if (dlg->ShowModal() == wxID_OK)
        {
            wxString text = dlg->getText();
            if (!text.IsEmpty())
            {
                addRectUV(text.ToUTF8().data(), RectUV(0, 0, 1, 1), RectState::State_Selected);
            }
        }
    }
    else if (event.GetId() == idMenuRenameItem)
    {
        wxArrayInt selections;
        list->GetSelections(selections);

        if (selections.GetCount() == 1)
        {
            RectState* st = (RectState*)list->GetClientData(selections[0]);

            if (st)
            {
                NameDialog* dlg = new NameDialog(this, wxID_ANY, _("Rename"));
                dlg->setText(wxString::FromUTF8(st->name.c_str()));
                if (dlg->ShowModal() == wxID_OK)
                {
                    wxString text = dlg->getText();
                    if (!text.IsEmpty())
                    {
                        list->SetString(selections[0], text);
                        st->name = text.ToUTF8().data();
                    }
                }
            }
        }
    }
    else if (event.GetId() == idMenuDelItem)
    {
        wxArrayInt selections;
        list->GetSelections(selections);

        if (selections.GetCount() > 0)
        {
            for (int i = selections.GetCount() - 1; i >= 0; --i)
            {
                RectState* st = (RectState*)list->GetClientData(selections[i]);
                state->rects.remove(st);
                list->Delete(selections[i]);
            }
        }
    }
    else
        event.Skip();
}

void UVRCEditorPage::onRightClick(wxMouseEvent& event)
{
    wxMenu *menu = new wxMenu(wxEmptyString);
    menu->Append(idMenuAddItem, _("&Add new ..."));

    wxArrayInt selections;
    list->GetSelections(selections);
    
    if (selections.GetCount() == 1)
    {
        menu->Append(idMenuRenameItem, _("&Rename ..."));
    }

    if (selections.GetCount() > 0)
    {
        menu->Append(idMenuDelItem, _("&Delete"));
    }

    if (menu->GetMenuItemCount() != 0)
        PopupMenu(menu);

    delete menu;

    event.Skip();
}

UVRCMIMEHandler::UVRCMIMEHandler()
{
}

bool UVRCMIMEHandler::newPage(UVEditorState* state, bool isModified)
{
    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    epm->getNotebook()->Freeze();
    UVRCEditorPage* page = new UVRCEditorPage(epm->getNotebook(), this);
    epm->addEditorPage(page);
    epm->getNotebook()->Thaw();

    page->setUVState(state);

    if (isModified)
        page->setModified(isModified);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return true;
}

bool UVRCMIMEHandler::canHandle(const wxString& filename) const
{
    return filename.Lower().EndsWith(wxT(".uvrc"));
}

bool UVRCMIMEHandler::openFile(const wxString& filename)
{
    String fileName = filename.ToUTF8().data();
    UVEditorState::Ptr uvState = (UVEditorState*)Object::importObject(fileName);

    if (!uvState)
        return false;

    return newPage(uvState, false);
}
