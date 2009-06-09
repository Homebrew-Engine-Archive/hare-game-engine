//***************************************************************
//  File:    TextEditorPage.h
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
#ifndef _TEXTEDITORPAGE_H_
#define _TEXTEDITORPAGE_H_

#include "EditorPrerequisites.h"
#include "EditorPage.h"
#include "ProjectFile.h"

namespace hare
{

#define ERROR_MARKER        1
#define ERROR_STYLE         wxSCI_MARK_SMALLRECT
#define BOOKMARK_MARKER     2
#define BOOKMARK_STYLE      wxSCI_MARK_ROUNDRECT
#define BREAKPOINT_MARKER   3
#define BREAKPOINT_STYLE    wxSCI_MARK_CIRCLE
#define DEBUG_MARKER        4
#define DEBUG_STYLE         wxSCI_MARK_ARROW

#define MARGIN_LINENUMBER   0
#define MARGIN_SYMBOL       1
#define MARGIN_FOLDING      2


    class EDITOR_API TextEditorPage : public EditorPage
    {
    public:
        TextEditorPage(wxWindow* parent, const wxString& fullFileName);

        const wxString& getFileName();
        wxString getEncodingName();

        void setTextEditorTitle(const wxString& str);

        wxScintilla* getControl() const;

        bool getIsOK() const;

        bool open(bool detectEncoding = true);
        virtual bool save();
        bool saveAs();

        void gotoLine(int line);
        void setDebugLine(int line);

        void foldLine(int line);
        void unfoldLine(int line);

        int getLineCount();
        void markLine(int marker, int line);
        bool lineHasMarker(int marker, int line) const;
        void toggleMarker(int marker, int line);

        void toggleBreakPoint(int line);

        void setFoldingIndicator(int id);
        void setMarkerStyle(int marker, int markerType, wxColor fore, wxColor back);

        void setProjectFile(ProjectFile* prjFile);
        ProjectFile* getProjectFile();

    public:
        virtual bool getIsModified() const;
        virtual void setModified(bool modified = true);

        virtual void undo();
        virtual void redo();
        virtual void cut();
        virtual void copy();
        virtual void paste();
        virtual void selectAll();

        virtual bool canUndo() const;
        virtual bool canRedo() const;
        virtual bool canPaste() const;
        virtual bool hasSelection() const;
        virtual bool isReadOnly() const;

    public:
        static void setDefaultEditorStyle(wxScintilla* control);

    private:
        wxScintilla* createTextEditor();

        void onMarginClick(wxScintillaEvent& event);
        void onEditorUpdateUI(wxScintillaEvent& event);
        void onEditorChange(wxScintillaEvent& event);
        void onEditorModified(wxScintillaEvent& event);

        void updateLineNumberColWidth();

        void addBreakPoint(const String& file, int line);
        void removeBreakPoint(const String& file, int line);

    private:
        wxScintilla* editorCtrl;
        int editorID;
        wxString fileName;
        wxString shortName;
        wxFontEncoding encoding;
        bool useBOM;
        wxBoxSizer* boxSizer;
        bool isOK;
        bool isModified;
        wxDateTime lastModified;

        ProjectFile::Ptr projectFile;

        DECLARE_ABSTRACT_CLASS(TextEditorPage)
    };
}

#endif
