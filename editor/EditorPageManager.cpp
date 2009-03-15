//***************************************************************
//  File:    EditorPageManager.cpp
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
#include "EditorPageManager.h"
#include "EditorManager.h"
#include "TextEditorPage.h"
#include <wx/utils.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#include <wx/filename.h>

namespace hare_editor
{
    template<> EditorPageManager* TManager<EditorPageManager>::instance = 0;
    template<> bool TManager<EditorPageManager>::autoCreate = true;

    static const int idEditorPageManager = wxNewId();

    EditorPageManager::EditorPageManager()
    {
        notebook = new wxFlatNotebook(Manager::getInstancePtr()->getAppWindow(), idEditorPageManager,
            wxDefaultPosition, wxDefaultSize, wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN | 
            wxFNB_DROPDOWN_TABS_LIST | wxFNB_NO_NAV_BUTTONS);

        textEditorStyle = new TextEditorStyle();
    }

    EditorPageManager::~EditorPageManager()
    {
        delete textEditorStyle;
        textEditorStyle = 0;
        notebook->Destroy();
    }

    TextEditorStyle* EditorPageManager::getTextEditorStyle()
    {
        return textEditorStyle;
    }

    void EditorPageManager::addEditorPage(EditorPage* page)
    {
        notebook->AddPage(page, page->getTitle(), true);
    }

    EditorPage* EditorPageManager::getActivePage()
    {
        int page = notebook->GetSelection();
        return static_cast<EditorPage*>(notebook->GetPage(page));
    }

    bool EditorPageManager::setPageText(EditorPage* page, const wxString& text)
    {
        for (int i = 0; i < notebook->GetPageCount(); ++i)
        {
            if (page == notebook->GetPage(i))
            {
                notebook->SetPageText(i, text);
                return true;
            }
        }

        return false;
    }

    TextEditorPage* EditorPageManager::openInTextEditor(const wxString& fileName)
    {
        wxFileName fname(fileName);
        fname.Normalize();

        if (!fname.FileExists())
            return 0;

        for (int i = 0; i < notebook->GetPageCount(); ++i)
        {
            wxWindow* page = notebook->GetPage(i);
            if (wxIsKindOf(page, TextEditorPage))
            {
                TextEditorPage* editor = static_cast<TextEditorPage*>(page);

                if (wxFileName(editor->getFileName()) == fname)
                {
                    notebook->SetSelection(i);
                    return editor;
                }
            }
        }

        notebook->Freeze();
        TextEditorPage* editor = new TextEditorPage(notebook, fname.GetFullPath());
        notebook->Thaw();
        addEditorPage(editor);

        return editor;
    }

    int EditorPageManager::getPageCount()
    {
        return notebook->GetPageCount();
    }

    EditorPage* EditorPageManager::getPage(int index)
    {
        return (EditorPage*)notebook->GetPage(index);
    }

}
