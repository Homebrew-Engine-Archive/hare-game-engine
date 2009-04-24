//***************************************************************
//  File:    EditorMain.h
//  Date:    10/20/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef EDITORMAIN_H
#define EDITORMAIN_H

#include "editor/Editor.h"
#include <wx/aui/aui.h>

WX_DECLARE_HASH_MAP(int, wxAuiToolBar*, wxIntegerHash, wxIntegerEqual, PluginToolBarMap);

class EditorFrame: public wxFrame
{
public:
    EditorFrame(wxFrame *frame, const wxString& title, const wxString& scriptDir);
   ~EditorFrame();
private:
    wxAuiManager layoutManager;
    wxAuiToolBar* mainToolBar;
    wxAuiToolBar* fullScreenToolBar;
    FindReplaceDialog* findReplaceDlg;

    PluginToolBarMap pluginToolBars;

    void preLoadXRC();

    void registerEvents();
    void createIDE();
    void createToolBar();
    void createMenuBar();
    void createStatusBar();

public:
    // event handlers
    void onEraseBackground(wxEraseEvent& event);
    void onSize(wxSizeEvent& event);
    void onApplicationClose(wxCloseEvent& event);
    // file menu
    void onFileNew(wxCommandEvent& event);
    void onFileOpen(wxCommandEvent& event);
    void onFileReopenProject(wxCommandEvent& event);
    void onFileOpenRecentProjectClearHistory(wxCommandEvent& event);
    void onFileReopen(wxCommandEvent& event);
    void onFileOpenRecentClearHistory(wxCommandEvent& event);
    void onFileSave(wxCommandEvent& event);
    void onFileSaveAll(wxCommandEvent& event);
    void onFileQuit(wxCommandEvent& event);
    // edit menu
    void onEditUndo(wxCommandEvent& event);
    void onEditRedo(wxCommandEvent& event);
    void onEditCut(wxCommandEvent& event);
    void onEditCopy(wxCommandEvent& event);
    void onEditPaste(wxCommandEvent& event);
    void onEditSelectAll(wxCommandEvent& event);
    void onEditFind(wxCommandEvent& event);
    void onEditFindInFile(wxCommandEvent& event);
    void onEditGoto(wxCommandEvent& event);

    void onShowToolBar(wxCommandEvent& event);
    void onToggleFullScreen(wxCommandEvent& event);
    bool onDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

    void onToolbarDropDownCreate(wxAuiToolBarEvent& event);
    
    void onFileMenuUpdateUI(wxUpdateUIEvent& event);
    void onEditMenuUpdateUI(wxUpdateUIEvent& event);

    void onEditorUpdateUI(EditorEvent& event);
    void onEditorActivated(EditorEvent& event);
    void onEditorClose(EditorEvent& event);

    void onPluginAttached(EditorEvent& event);
    void onLayoutSwitch(EditorEvent& event);
    void onLayoutQuery(EditorEvent& event);

    void onAddDockWindow(EditorDockEvent& event);
    void onDelDockWindow(EditorDockEvent& event);
    void onShowDockWindow(EditorDockEvent& event);

private:
    void _doOpenFile();
    void _doUpdateStatusBar();

    DECLARE_EVENT_TABLE()
};


#endif // EDITORMAIN_H
