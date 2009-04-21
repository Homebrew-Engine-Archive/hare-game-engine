//***************************************************************
//  File:    ThemeMIMEHandler.cpp
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
#include "ThemeMIMEHandler.h"
#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

int idThemeWindow = wxNewId();

IMPLEMENT_ABSTRACT_CLASS(ThemeEditorPage, EditorPage)

ThemeEditorPage::ThemeEditorPage(wxWindow* parent, ThemeMIMEHandler* handler, Theme* theme)
 : UVEditorPage(parent), mime(handler)
{
    Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ThemeEditorPage::onEraseBackground), NULL, this);

    button = new Button;

    if (theme)
    {
        AttVisitor v;
        theme->accept(v);

        Attribute::List::iterator it = v.attributes.begin();
        for (; it != v.attributes.end(); ++it)
        {
            Attribute *attr = *it;

            if (attr->attrType == Attribute::attrMeta && attr->typeName == String("RectUV"))
            {
                RectUV* rect = (RectUV*)attr->data;
                addRectUV(attr->name, *rect, RectState::State_None);
            }
        }
    }

    Material* mtrl = (Material*)Object::importObject("default.material");
    setMaterial(mtrl);
}

ThemeEditorPage::~ThemeEditorPage()
{
    if (themePtr)
    {
        if (!Manager::isAppShuttingDown())
        {
            Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        }
    }

    mime->page = NULL;
}

void ThemeEditorPage::onSize(wxSizeEvent& event)
{
}

void ThemeEditorPage::onMouseMove(wxMouseEvent& event)
{
}

void ThemeEditorPage::onEraseBackground(wxEraseEvent& event)
{
}

ThemeMIMEHandler::ThemeMIMEHandler() : page(0)
{
}

bool ThemeMIMEHandler::canHandle(const wxString& filename) const
{
    return filename.Lower().EndsWith(wxT(".theme"));
}

bool ThemeMIMEHandler::openFile(const wxString& filename)
{
    Theme::Ptr theme = (Theme*)Object::importObject(filename.ToUTF8().data());

    if (!theme)
        return false;

    if (!page)
    {
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Freeze();
        page = new ThemeEditorPage(Manager::getInstancePtr()->getEditorPageManager()->getNotebook(), this, theme);
        Manager::getInstancePtr()->getEditorPageManager()->addEditorPage(page);
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->Thaw();
    }

    int index = Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->GetPageIndex(page);

    if (index != -1)
        Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->SetSelection(index);

    return page->isOk();
}
