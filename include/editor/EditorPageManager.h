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

namespace hare_editor
{
    class EDITOR_API EditorPageManager : public TManager<EditorPageManager>
    {
        friend class TManager<EditorPageManager>;

    public:
        wxFlatNotebook* getNotebook() { return notebook; }

        void addEditorPage(EditorPage* page);

        TextEditorPage* openInTextEditor(const wxString& fileName);

        EditorPage* getActivePage();

        TextEditorStyle* getTextEditorStyle();

        bool setPageText(EditorPage* page, const wxString& text);

        int getPageCount();

        EditorPage* getPage(int index);

    private:

        wxFlatNotebook* notebook;
        TextEditorStyle* textEditorStyle;

    private:
         EditorPageManager();
        ~EditorPageManager();
    };
}

#endif