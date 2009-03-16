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

namespace hare_editor
{
    class EDITOR_API TextEditorPage : public EditorPage
    {
    public:
        TextEditorPage(wxWindow* parent, const wxString& fullFileName);

        const wxString& getFileName();
        wxString getEncodingName();

        void setTextEditorTitle(const wxString& str);

        wxScintilla* getControl() const;

        bool getIsOK() const;
        bool getIsModified() const;
        void setModified(bool modified = true);

        bool open(bool detectEncoding = true);
        virtual bool save();
        bool saveAs();

        void gotoLine(int line);
        void setDebugLine(int line);

        void foldLine(int line);
        void unfoldLine(int line);

        void markLine(int marker, int line);
        bool lineHasMarker(int marker, int line) const;
        void toggleMarker(int marker, int line);

        void toggleBreakPoint(int line);

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

        void setFoldingIndicator(int id);
        void setMarkerStyle(int marker, int markerType, wxColor fore, wxColor back);

        void setProjectFile(ProjectFile* prjFile);
        ProjectFile* getProjectFile();

    public:
        static void setDefaultEditorStyle(wxScintilla* control);

    private:
        wxScintilla* createTextEditor();

        void onMarginClick(wxScintillaEvent& event);
        void onEditorUpdateUI(wxScintillaEvent& event);
        void onEditorChange(wxScintillaEvent& event);
        void onEditorModified(wxScintillaEvent& event);

        void updateLineNumberColWidth();

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
    };
}

#endif