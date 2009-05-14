//***************************************************************
//  File:    MtrlWizard.cpp
//  Data:    04/29/2009
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
#include "MtrlWizard.h"
#include "MtrlMIMEHandler.h"
#include <wx/wizard.h>

class wxMaterialWizardPage : public wxWizardPageSimple
{
public:
    wxMaterialWizardPage(wxWizard* parent) : wxWizardPageSimple(parent)
    {
        wxBoxSizer* boxSizer1 = new wxBoxSizer(wxVERTICAL);
        {
            wxStaticText* staticText = new wxStaticText(this, wxID_ANY,
                _T("This wizard helps you to create materials."), wxPoint(5,5));
            boxSizer1->Add(staticText, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }
        {
            wxStaticText* staticText = new wxStaticText(this, wxID_ANY, _("File name:"));
            boxSizer1->Add(staticText, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
            txtFileName = new wxTextCtrl(this, wxID_ANY);
            txtFileName->Disable();
            boxSizer1->Add(txtFileName, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }
        {
            wxStaticText* staticText = new wxStaticText(this, wxID_ANY, _("File ext:"));
            boxSizer1->Add(staticText, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
            txtFileExt = new wxTextCtrl(this, wxID_ANY);
            txtFileExt->Disable();
            boxSizer1->Add(txtFileExt, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }
        {
            wxStaticText* staticText = new wxStaticText(this, wxID_ANY, _("Folder to create material in:"));
            boxSizer1->Add(staticText, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
            txtPath = new wxTextCtrl(this, wxID_ANY);
            txtPath->Disable();
            wxBoxSizer* boxSizer2 = new wxBoxSizer(wxHORIZONTAL);
            boxSizer2->Add(txtPath, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
            btnBrowse = new wxButton(this, wxID_ANY, wxT("..."), 
                wxDefaultPosition, wxSize(22,22));
            btnBrowse->Disable();
            boxSizer2->Add(btnBrowse, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
            boxSizer1->Add(boxSizer2, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }
        {
            chkNoSave = new wxCheckBox(this, wxID_ANY, _T("Create for editing, do not save to file now."));
            chkNoSave->SetValue(true);
            boxSizer1->Add(chkNoSave, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }
        SetSizer(boxSizer1);
        boxSizer1->Fit(this);
        boxSizer1->SetSizeHints(this);

        txtFileExt->SetValue(wxT(".material"));
        txtPath->SetValue(wxT("/"));
        Connect(wxEVT_WIZARD_PAGE_CHANGING, wxWizardEventHandler(wxMaterialWizardPage::onWizardPageChanging), 0, this);
        btnBrowse->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(wxMaterialWizardPage::onBrowse), 0, this);
        chkNoSave->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(wxMaterialWizardPage::onCheckChanged), 0, this);
    }

    bool getIsChecked()
    {
        return chkNoSave->IsChecked();
    }

    wxString getResult()
    {
        return txtPath->GetValue() + txtFileName->GetValue() + txtFileExt->GetValue();
    }

    void onWizardPageChanging(wxWizardEvent& event)
    {
        if (chkNoSave->IsChecked())
            return;
        else if (txtPath->GetValue().IsEmpty() ||
            txtFileName->GetValue().IsEmpty())
        {
            wxMessageBox(_T("File name or folder can not be empty."), _T("Not allowed"),
                wxICON_WARNING | wxOK, this);
            event.Veto();
        }
    }

    void onBrowse(wxCommandEvent& event)
    {
        FileSystemDialog dlg(this, true, _("FileSystem"), getResult(), 
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, 
            wxDefaultPosition, wxSize(400, 400));

        if (dlg.ShowModal() == wxID_OK)
        {
            txtPath->SetValue(dlg.GetDir());
        }
    }

    void onCheckChanged(wxCommandEvent& event)
    {
        bool enable = !chkNoSave->IsChecked();
        txtFileName->Enable(enable);
        txtFileExt->Enable(enable);
        btnBrowse->Enable(enable);
    }

private:
    wxTextCtrl* txtFileName;
    wxTextCtrl* txtFileExt;
    wxTextCtrl* txtPath;
    wxButton* btnBrowse;
    wxCheckBox* chkNoSave;
};

class wxMaterialWizard : public wxWizard
{
public:
    wxMaterialWizard(wxWindow* parent, const wxString& title, wxBitmap& bitmap)
        : wxWizard(parent, wxID_ANY, title, bitmap, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    {
        page = new wxMaterialWizardPage(this);
        SetPageSize(wxSize(400, 300));
    }

    wxWizardPageSimple* GetFirstPage()
    {
        return page;
    }

    wxString getResult()
    {
        return page->getResult();
    }

    bool getIsChecked()
    {
        return page->getIsChecked();
    }

private:
    wxMaterialWizardPage* page;
};

const ClassInfo* mtrlClasses[] = {
    &PannerMod::CLASS_INFO,
    &ScalerMod::CLASS_INFO,
    &RotatorMod::CLASS_INFO,
    &SimpleShader::CLASS_INFO,
    &ParticleShader::CLASS_INFO,
};

MtrlWizard::MtrlWizard()
{
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
    bitmap.LoadFile(fullPath + wxT("file32.png"), wxBITMAP_TYPE_PNG);
    wizardBitmap.LoadFile(fullPath + wxT("mtrl_edit.png"), wxBITMAP_TYPE_PNG);
}

wxString MtrlWizard::getFolder() const
{
    return wxT("Materials");
}

const wxBitmap& MtrlWizard::getBitmap(int index) const
{
    return bitmap;
}

wxString MtrlWizard::getTitle(int index) const
{
    return wxString::FromUTF8(mtrlClasses[index]->className);
}

wxString MtrlWizard::getDesc(int index) const
{
    return wxString::FromUTF8(mtrlClasses[index]->className);
}

int MtrlWizard::getCount() const
{
    return sizeof(mtrlClasses) / sizeof(mtrlClasses[0]);
}

Object* MtrlWizard::wizard(int index)
{
    wxMaterialWizard wizard(Manager::getInstancePtr()->getAppWindow(), _("Material Wizard [create material]"),
        wizardBitmap);

    if (wizard.RunWizard(wizard.GetFirstPage()))
    {
        Object::Ptr object = mtrlClasses[index]->createObject();

        if (object)
        {
            EditorPlugin* plugin = PluginManager::getInstancePtr()->findPluginByName(wxT("MtrlMIMEHandler"));
            if (plugin && plugin->getType() == EPT_MIMEHandler)
            {
                MtrlMIMEHandler* handler = (MtrlMIMEHandler*)plugin;
                if (wizard.getIsChecked())
                {
                    handler->addMaterial((Material*)object.pointer(), true);
                }
                else
                {
                    String fileName = wizard.getResult().ToUTF8().data();
                    if (object->saveToXml(fileName))
                    {
                        handler->openFile(wizard.getResult());
                        return object;
                    }
                }
            }
        }
    }

    return NULL;
}