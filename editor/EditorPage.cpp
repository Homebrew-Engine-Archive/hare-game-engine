//***************************************************************
//  File:    EditorPage.cpp
//  Data:    11/26/2008
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
#include "EditorPage.h"
#include "EditorManager.h"
#include "EditorPageManager.h"

namespace hare
{
    IMPLEMENT_ABSTRACT_CLASS(EditorPage, wxPanel)

    EditorPage::EditorPage()
    {
        // This ctor is used for creating Panel form xrc resource 
    }

    EditorPage::EditorPage(wxWindow* parent)
        : wxPanel(parent)
    {
    }

    const wxString& EditorPage::getTitle()
    {
        return title;
    }

    void EditorPage::setTitle(const wxString& str)
    {
        title = str;
        Manager::getInstancePtr()->getEditorPageManager()->setPageText(this, title);
    }
}
