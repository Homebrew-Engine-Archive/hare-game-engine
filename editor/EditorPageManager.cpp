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
#include <wx/filename.h>

namespace hare
{
    BEGIN_EVENT_TABLE(EditorNotebook, wxFlatNotebook)
        EVT_FLATNOTEBOOK_PAGE_CHANGING(wxID_ANY, EditorNotebook::onPageChanging)
        EVT_FLATNOTEBOOK_PAGE_CHANGED(wxID_ANY, EditorNotebook::onPageChanged)
        EVT_FLATNOTEBOOK_PAGE_CLOSING(wxID_ANY, EditorNotebook::onPageClosing)
    END_EVENT_TABLE()

    EditorNotebook::EditorNotebook(wxWindow* parent, wxWindowID id, 
        const wxPoint& pos, const wxSize& size, long style, const wxString& name) 
        : wxFlatNotebook(parent, id, pos, size, style, name)
    {
    }

    void EditorNotebook::onPageChanged(wxFlatNotebookEvent& event)
    {
        EditorPage* page = (EditorPage*)GetPage(event.GetSelection());
        EditorEvent evt(editorEVT_EDITOR_ACTIVATED);
        evt.editorPage = page;
        Manager::getInstancePtr()->processEvent(evt);

        event.Skip(); // allow others to process it too
    }

    void EditorNotebook::onPageChanging(wxFlatNotebookEvent& event)
    {
        EditorPage* page = (EditorPage*)GetPage(event.GetOldSelection());
        EditorEvent evt(editorEVT_EDITOR_DEACTIVATED);
        evt.editorPage = page;
        Manager::getInstancePtr()->processEvent(evt);

        event.Skip(); // allow others to process it too
    }

    void EditorNotebook::onPageClosing(wxFlatNotebookEvent& event)
    {
        EditorPage* page = (EditorPage*)GetPage(event.GetSelection());

        if (page && page->getIsModified())
        {
            if (!EditorPageManager::getInstancePtr()->queryClosePage(page))
                event.Veto();
        }

        event.Skip(); // allow others to process it too
    }

    template<> EditorPageManager* TManager<EditorPageManager>::instance = 0;
    template<> bool TManager<EditorPageManager>::autoCreate = true;

    static const int idEditorPageFNB = wxNewId();

    EditorPageManager::EditorPageManager()
    {
        notebook = new EditorNotebook(Manager::getInstancePtr()->getAppWindow(), idEditorPageFNB,
            wxDefaultPosition, wxDefaultSize, wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN |
            wxFNB_DROPDOWN_TABS_LIST | wxFNB_NO_NAV_BUTTONS | wxFNB_VC8 | wxFNB_DCLICK_CLOSES_TABS);

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

    bool EditorPageManager::queryClosePage(EditorPage* page)
    {
        if (!page)
            return true;

        if (!page->getIsModified())
            return true;

        wxString msg;
        msg.Printf(_("Page '%s' is modified...\nDo you want to save the changes?"), 
            page->getTitle().c_str());

        wxMessageDialog dlg(page, msg, _("Save Changes"), wxCENTER | wxICON_QUESTION | wxYES_NO | wxCANCEL);

        switch (dlg.ShowModal())
        {
        case wxID_YES:
            return page->save();
            break;
        case wxID_NO:
            page->setModified(false);
            return true;
        case wxCANCEL:
        default:
            return false;
        }
        return false;
    }

    bool EditorPageManager::queryCloseAllPages()
    {
        for (int i = 0; i < notebook->GetPageCount(); ++i)
        {
            EditorPage* page = (EditorPage*)notebook->GetPage(i);
            if (page->getIsModified())
            {
                if (!queryClosePage(page))
                    return false;
            }
        }
        return true;
    }

    bool EditorPageManager::saveAll()
    {
        bool alldone = true;

        for (int i = 0; i < notebook->GetPageCount(); ++i)
        {
            EditorPage* page = (EditorPage*)notebook->GetPage(i);
            if (page->getIsModified())
            {
                alldone = page->save() && alldone;
            }
        }
        return alldone;
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
