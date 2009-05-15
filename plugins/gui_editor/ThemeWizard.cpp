//***************************************************************
//  File:    ThemeWizard.cpp
//  Data:    05/14/2009
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
#include "ThemeWizard.h"
#include "ThemeMIMEHandler.h"
#include <wx/wizard.h>
#include <wx/propgrid/propgrid.h>

class wxThemeWizardPage : public wxWizardPageSimple
{
public:
    wxThemeWizardPage(wxWizard* parent) : wxWizardPageSimple(parent)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        grid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED);

        sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Select a theme for each window : ")), 0, wxEXPAND | wxALL, 5);
        sizer->Add(grid, 1, wxEXPAND | wxALL, 5);

        SetSizer(sizer);
        Layout();

        Window::CLASS_INFO.findSubs(windowClasses);
        Theme::CLASS_INFO.findSubs(themeClasses);

        themes.resize(themeClasses.size());
        for (size_t i = 0; i < themeClasses.size();  ++i)
        {
            themes[i] = (Theme*)themeClasses[i]->createObject();
        }

        for (size_t i = 0; i < windowClasses.size();  ++i)
        {
            wxPGChoices choices;
            choices.Add(wxT("<NullOject>"), -100);
            
            wxPGProperty* prop = grid->Append(new wxEnumProperty(wxString::FromUTF8(windowClasses[i]->className)));
            
            for (size_t j = 0; j < themes.size(); ++j)
            {
                if (windowClasses[i]->isDerivedFrom(themes[j]->getWindowClass()))
                {
                    choices.Add(wxString::FromUTF8(themes[j]->getClassInfo()->className), j);
                }
            }
            prop->SetChoices(choices);
            prop->SetValue(-100);
        }

        Connect(wxEVT_WIZARD_PAGE_CHANGING, wxWizardEventHandler(wxThemeWizardPage::onWizardPageChanging), 0, this);
    }

    void onWizardPageChanging(wxWizardEvent& event)
    {
        themePackage = new ThemePackage();

        wxPropertyGridIterator it = grid->GetIterator();
        while (!it.AtEnd())
        {
            wxPGProperty* prop = it.GetProperty();

            int sel = wxPGVariantToInt(prop->GetValue());

            if (sel >= 0)
            {
                themePackage->getThemes().push_back(themes[sel]);
            }

            it.Next();
        }
    }

    ThemePackage* getResult()
    {
        return themePackage;
    }

private:
    wxPropertyGrid* grid;
    ThemePackage::Ptr themePackage;
    ClassInfoList windowClasses;
    ClassInfoList themeClasses;
    Theme::Array themes;
};

class wxThemeWizard : public wxWizard
{
public:
    wxThemeWizard(wxWindow* parent, const wxString& title, wxBitmap& bitmap)
        : wxWizard(parent, wxID_ANY, title, bitmap, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    {
        page = new wxThemeWizardPage(this);
        SetPageSize(wxSize(400, 300));
    }

    wxWizardPageSimple* GetFirstPage()
    {
        return page;
    }

    ThemePackage* getResult()
    {
        return page->getResult();
    }

private:
    wxThemeWizardPage* page;
};


ThemeWizard::ThemeWizard()
{
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
    bitmap.LoadFile(fullPath + wxT("file32.png"), wxBITMAP_TYPE_PNG);
    wizardBitmap.LoadFile(fullPath + wxT("mtrl_edit.png"), wxBITMAP_TYPE_PNG);
}

wxString ThemeWizard::getFolder() const
{
    return wxT("ThemePackage");
}

const wxBitmap& ThemeWizard::getBitmap(int index) const
{
    return bitmap;
}

wxString ThemeWizard::getTitle(int index) const
{
    return wxT("ThemePackage");
}

wxString ThemeWizard::getDesc(int index) const
{
    return wxT("Create a theme package");
}

int ThemeWizard::getCount() const
{
    return 1;
}

Object* ThemeWizard::wizard(int index)
{
    wxThemeWizard wizard(Manager::getInstancePtr()->getAppWindow(), _("Theme Wizard [create theme package]"),
        wizardBitmap);

    if (wizard.RunWizard(wizard.GetFirstPage()))
    {
        ThemePackage::Ptr themes = wizard.getResult();

        if (themes)
        {
            EditorPlugin* plugin = PluginManager::getInstancePtr()->findPluginByName(wxT("ThemeMIMEHandler"));
            if (plugin && plugin->getType() == EPT_MIMEHandler)
            {
                ThemeMIMEHandler* handler = (ThemeMIMEHandler*)plugin;
                handler->newPage(themes, true);
            }
        }
    }

    return NULL;
}