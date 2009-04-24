//***************************************************************
//  File:    EditorPage.h
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
#ifndef _EDITOR_PAGE_H_
#define _EDITOR_PAGE_H_

#include "EditorPrerequisites.h"
#include <wx/panel.h>

namespace hare
{
    class EDITOR_API EditorPage : public wxPanel
    {
    public:
        EditorPage();
        EditorPage(wxWindow* parent);
        virtual ~EditorPage();

        const wxString& getTitle();
        void setTitle(const wxString& str);

        virtual bool save() { return true; }
        virtual bool getIsModified() const { return false; }
        virtual void setModified(bool modified = true) {}

        virtual void undo() {}
        virtual void redo() {}
        virtual void cut() {}
        virtual void copy() {}
        virtual void paste() {}
        virtual void selectAll() {}

        virtual bool canUndo() const { return false; }
        virtual bool canRedo() const { return false; }
        virtual bool canPaste() const { return false; }
        virtual bool hasSelection() const { return false; }
        virtual bool isReadOnly() const { return true; }

    protected:
        wxString title;

        DECLARE_ABSTRACT_CLASS(EditorPage)
    };
}

#endif
