//***************************************************************
//  File:    EditorPageManager.h
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
#ifndef _EDITOR_PAGE_MANAGER_H_
#define _EDITOR_PAGE_MANAGER_H_

#include "EditorPrerequisites.h"
#include "TManager.h"
#include "TextEditorStyle.h"
#include <wx/string.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

namespace hare
{
    class EDITOR_API EditorNotebook : public wxFlatNotebook
    {
    public:
        EditorNotebook(wxWindow* parent, wxWindowID id = wxID_ANY, 
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
            long style = 0, const wxString& name = wxT("Editor Notebook"));

    private:
        void onPageChanging(wxFlatNotebookEvent& event);
        void onPageChanged(wxFlatNotebookEvent& event);
        void onPageClosing(wxFlatNotebookEvent& event);

        DECLARE_EVENT_TABLE()
    };

    class EDITOR_API EditorPageManager : public TManager<EditorPageManager>
    {
        friend class TManager<EditorPageManager>;

    public:
        EditorNotebook* getNotebook() { return notebook; }

        void addEditorPage(EditorPage* page);

        TextEditorPage* openInTextEditor(const wxString& fileName);

        EditorPage* getActivePage();

        TextEditorStyle* getTextEditorStyle();

        bool setPageText(EditorPage* page, const wxString& text);

        int getPageCount();

        EditorPage* getPage(int index);

        bool queryClosePage(EditorPage* page);
        bool queryCloseAllPages();

        bool saveAll();

    private:
        EditorNotebook* notebook;
        TextEditorStyle* textEditorStyle;

    private:
        EditorPageManager();
       ~EditorPageManager();
    };
}

#endif
