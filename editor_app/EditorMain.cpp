//***************************************************************
//  File:    EditorMain.cpp
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
#include "PCH.h"
#include "EditorMain.h"
#include <wx/wxscintilla.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#include <wx/harecanvas.h>
#include <wx/xrc/xmlres.h>

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
#include "editor.xpm"
#endif

class FileDropTarget : public wxFileDropTarget
{
public:
    FileDropTarget(EditorFrame *f) : frame(f)
    {}
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
    {
        if (!frame) return false;
        return frame->onDropFiles(x, y, filenames);
    }
private:
    EditorFrame* frame;
};

int wxID_FILE10 = wxNewId();
int wxID_FILE11 = wxNewId();
int wxID_FILE12 = wxNewId();
int wxID_FILE13 = wxNewId();
int wxID_FILE14 = wxNewId();
int wxID_FILE15 = wxNewId();
int wxID_FILE16 = wxNewId();
int wxID_FILE17 = wxNewId();
int wxID_FILE18 = wxNewId();
int wxID_FILE19 = wxNewId();

int idFileNew = XRCID("idFileNew");
int idFileOpen = XRCID("idFileOpen");
int idFileClose = XRCID("idFileClose");
int idFileSave = XRCID("idFileSave");
int idFileSaveAll = XRCID("idFileSaceAll");
int idFileReopenFile = XRCID("idFileReopenFile");
int idFileOpenRecentFileClearHistory = XRCID("idFileOpenRecentFileClearHistory");
int idFileExit = XRCID("idFileExit");

int idEditUndo = XRCID("idEditUndo");
int idEditRedo = XRCID("idEditRedo");
int idEditCut = XRCID("idEditCut");
int idEditCopy = XRCID("idEditCopy");
int idEditPaste = XRCID("idEditPaste");
int idEditSelectAll = XRCID("idEditSelectAll");
int idEditFind = XRCID("idEditFind");
int idEditFindInFile = XRCID("idEditFindInFile");
int idEditGoto = XRCID("idEditGoto");

int idViewExplorer = XRCID("idViewExplorer");
int idViewToolMain = XRCID("idViewToolMain");
int idViewToolbars = XRCID("idViewToolbars");
int idViewToolFullScreen = XRCID("idViewToolFullScreen");

int idViewFullScreen = XRCID("idViewFullScreen");

int idDebugStart = XRCID("idDebugStart");

int idMainToolbarChoice = wxNewId();    // select the active project

// add more IDs here

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_ERASE_BACKGROUND(EditorFrame::onEraseBackground)
    EVT_SIZE(EditorFrame::onSize)
    EVT_CLOSE(EditorFrame::onApplicationClose)
    
    EVT_UPDATE_UI(idFileOpenRecentFileClearHistory, EditorFrame::onFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveAll, EditorFrame::onFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileClose, EditorFrame::onFileMenuUpdateUI)

    EVT_UPDATE_UI(idEditUndo, EditorFrame::onEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditRedo, EditorFrame::onEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCopy, EditorFrame::onEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCut, EditorFrame::onEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditPaste, EditorFrame::onEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditSelectAll, EditorFrame::onEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditFind, EditorFrame::onEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditFindInFile, EditorFrame::onEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditGoto, EditorFrame::onEditMenuUpdateUI)

    EVT_MENU(idFileNew, EditorFrame::onFileNew)
    EVT_MENU(idFileOpen, EditorFrame::onFileOpen)
    EVT_MENU(idFileSave, EditorFrame::onFileSave)
    EVT_MENU(idFileSaveAll, EditorFrame::onFileSaveAll)
    EVT_MENU(idFileOpenRecentFileClearHistory, EditorFrame::onFileOpenRecentClearHistory)
    EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, EditorFrame::onFileReopen)
    EVT_MENU_RANGE(wxID_FILE10, wxID_FILE19, EditorFrame::onFileReopenProject)
    EVT_MENU(idFileExit, EditorFrame::onFileQuit)
    EVT_MENU(idEditUndo, EditorFrame::onEditUndo)
    EVT_MENU(idEditRedo, EditorFrame::onEditRedo)
    EVT_MENU(idEditCut, EditorFrame::onEditCut)
    EVT_MENU(idEditCopy, EditorFrame::onEditCopy)
    EVT_MENU(idEditPaste, EditorFrame::onEditPaste)
    EVT_MENU(idEditSelectAll, EditorFrame::onEditSelectAll)
    EVT_MENU(idEditFind, EditorFrame::onEditFind)
    EVT_MENU(idEditFindInFile, EditorFrame::onEditFindInFile)
    EVT_MENU(idEditGoto, EditorFrame::onEditGoto)
    EVT_MENU(idDebugStart, EditorFrame::onDebugStart)
    
    EVT_MENU(idViewToolMain, EditorFrame::onShowToolBar)
    EVT_MENU(idViewExplorer, EditorFrame::onShowToolBar)
    EVT_MENU(idViewToolFullScreen, EditorFrame::onShowToolBar)

    EVT_MENU(idViewFullScreen, EditorFrame::onToggleFullScreen)

    EVT_AUITOOLBAR_TOOL_DROPDOWN(idFileNew, EditorFrame::onToolbarDropDownCreate)
    EVT_AUITOOLBAR_TOOL_DROPDOWN(idDebugStart, EditorFrame::onToolbarDropDownDebug)
END_EVENT_TABLE()

EditorFrame::EditorFrame(wxFrame *frame, const wxString& title)
 : wxFrame(frame, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE), mainToolBar(0)
{
#ifdef __WXMSW__
    SetIcon(wxICON(_EXE_ICON_));
#else
    SetIcon(wxIcon(editor));
#endif // __WXMSW__

    FileSystem* fs = FileSystem::getSingletonPtr();
    // load resources for games
    ConfigFile resource;
    resource.load("resource.cfg");
    String scriptDir = resource.getSetting("ScriptDir");
    StringVector searchPaths = resource.getMultiSetting("SearchPath");
    for (size_t i = 0; i < searchPaths.size(); ++i)
    {
        wxFileName fn(wxString::FromUTF8(searchPaths[i].c_str()));
        if (fn.IsOk())
        {
            fn.Normalize();
            fs->addSearchPath(fn.GetFullPath().ToUTF8().data());
        }
    }

    // create canvas and destroy it, we mush have a window to init graphics moudle
    wxHareCanvas* canvas = new wxHareCanvas(this);
    canvas->Destroy();

    layoutManager.SetManagedWindow(this);

    registerEvents();

    wxXmlResource::Get()->InitAllHandlers();
    wxImage::AddHandler(new wxPNGHandler);

    SetDropTarget(new FileDropTarget(this));

    // Maximize and show the frame first
    Maximize(true);

    createIDE();

    wxString dir = wxString::FromUTF8(scriptDir.c_str());
    Manager::getInstancePtr()->getExplorerManager()->getProjectExplorer()->loadWorkspace(dir);

    wxString layout = Manager::getInstancePtr()->getConfigManager()->getAppConfigFile()->getLayout();

    if (!layout.IsEmpty())
        layoutManager.LoadPerspective(layout);
}

EditorFrame::~EditorFrame()
{
}

void EditorFrame::registerEvents()
{
    Manager* pm = Manager::getInstancePtr();

    pm->registerEvent(editorEVT_EDITOR_UPDATE_UI,
        new TEventHandler<EditorFrame, EditorEvent>(this, &EditorFrame::onEditorUpdateUI));
    pm->registerEvent(editorEVT_PLUGIN_ATTACHED,
        new TEventHandler<EditorFrame, EditorEvent>(this, &EditorFrame::onPluginAttached));
    pm->registerEvent(editorEVT_LAYOUT_SWITCH,
        new TEventHandler<EditorFrame, EditorEvent>(this, &EditorFrame::onLayoutSwitch));
    pm->registerEvent(editorEVT_LAYOUT_QUERY,
        new TEventHandler<EditorFrame, EditorEvent>(this, &EditorFrame::onLayoutQuery));

    pm->registerEvent(editorEVT_ADD_DOCK_WINDOW,
        new TEventHandler<EditorFrame, EditorDockEvent>(this, &EditorFrame::onAddDockWindow));
    pm->registerEvent(editorEVT_DEL_DOCK_WINDOW,
        new TEventHandler<EditorFrame, EditorDockEvent>(this, &EditorFrame::onDelDockWindow));
    pm->registerEvent(editorEVT_SHOW_DOCK_WINDOW,
        new TEventHandler<EditorFrame, EditorDockEvent>(this, &EditorFrame::onShowDockWindow));
}

void EditorFrame::createIDE()
{
    wxSize clientsize = wxGetClientDisplayRect().GetSize();

    preLoadXRC();
    createMenuBar();
    createToolBar();
    createStatusBar();

    Manager::getInstancePtr()->setAppWindow(this);

    // EditorPage Manager
    layoutManager.AddPane(Manager::getInstancePtr()->getEditorPageManager()->getNotebook(),
		wxAuiPaneInfo().Name(wxT("EditorPagePane")).CenterPane());
    
    // Explorer
    wxSize bestSize = wxSize(clientsize.GetWidth() / 5, clientsize.GetHeight() / 2);
    layoutManager.AddPane(Manager::getInstancePtr()->getExplorerManager()->getNotebook(),
        wxAuiPaneInfo().Name(wxT("ExplorerPane")).Caption(_("Explorer")).BestSize(bestSize).
        FloatingSize(bestSize).MinSize(wxSize(50, 100)).Left().CloseButton(true).MaximizeButton(true));

    findReplaceDlg = new FindReplaceDialog(this);

    wxString pluginPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/plugins/");

    Manager::getInstancePtr()->getPluginManager()->scanForPlugins(pluginPath);
    Manager::getInstancePtr()->getPluginManager()->loadAllPlugins();

    Manager::getInstancePtr()->getExplorerManager()->getNotebook()->SetDropTarget(new FileDropTarget(this));
    Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->SetDropTarget(new FileDropTarget(this));

    layoutManager.Update();
}

void EditorFrame::preLoadXRC()
{
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/resources/");
    wxXmlResource::Get()->Load(fullPath + wxT("/*.xrc"));
}

void EditorFrame::createMenuBar()
{
    wxMenuBar* mb = wxXmlResource::Get()->LoadMenuBar(wxT("main_menu_bar"));
    SetMenuBar(mb);
}

void EditorFrame::createToolBar()
{
    mainToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(16, 16),
        wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_HORZ_LAYOUT);
    mainToolBar->SetToolBitmapSize(wxSize(16, 16));

    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/resources/");
    wxBitmap bmp;

    bmp.LoadFile(fullPath + wxT("new.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idFileNew, _("New"), bmp, wxT("New"));
    mainToolBar->SetToolDropDown(idFileNew, true);
    bmp.LoadFile(fullPath + wxT("open.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idFileOpen, _("Open"), bmp, wxT("Open"));
    bmp.LoadFile(fullPath + wxT("save.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idFileSave, _("Save"), bmp, wxT("Save"));
    bmp.LoadFile(fullPath + wxT("save_all.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idFileSaveAll, _("Save All"), bmp, wxT("Save All"));
    mainToolBar->AddSeparator();
    bmp.LoadFile(fullPath + wxT("cut.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idEditCut, _("Cut"), bmp, _("Cut"));
    bmp.LoadFile(fullPath + wxT("copy.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idEditCopy, _("Copy"), bmp, _("Copy"));
    bmp.LoadFile(fullPath + wxT("paste.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idEditPaste, _("Paste"), bmp, _("Paste"));
    mainToolBar->AddSeparator();
    bmp.LoadFile(fullPath + wxT("undo.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idEditUndo, _("Undo"), bmp, _("Undo"));
    bmp.LoadFile(fullPath + wxT("redo.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idEditRedo, _("Redo"), bmp, _("Redo"));
    mainToolBar->AddSeparator();
    bmp.LoadFile(fullPath + wxT("find.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idEditFind, _("Find"), bmp, _("Find"));
    bmp.LoadFile(fullPath + wxT("find_in_file.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idEditFindInFile, _("FineInFile"), bmp, _("FineInFile"));
    mainToolBar->AddSeparator();
    bmp.LoadFile(fullPath + wxT("start.png"), wxBITMAP_TYPE_PNG);
    mainToolBar->AddTool(idDebugStart, _("Start"), bmp, _("Start"));
    mainToolBar->SetToolDropDown(idDebugStart, true);

    mainToolBar->AddControl(new wxChoice(mainToolBar, idMainToolbarChoice));

    mainToolBar->Realize();
    mainToolBar->SetInitialSize();

    layoutManager.AddPane(mainToolBar, wxAuiPaneInfo().Name(wxT("MainToolbar"))
        .Caption(wxT("Main Toolbar")).ToolbarPane().Top().LeftDockable(false).RightDockable(false));

    bmp.LoadFile(fullPath + wxT("full_screen.png"), wxBITMAP_TYPE_PNG);
    fullScreenToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(16, 16),
        wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORZ_TEXT);
    fullScreenToolBar->AddTool(idViewFullScreen, _("Full Screen"), bmp, wxEmptyString, wxITEM_CHECK);
    fullScreenToolBar->Realize();
    fullScreenToolBar->SetInitialSize();

    layoutManager.AddPane(fullScreenToolBar, wxAuiPaneInfo().Name(wxT("FullScreenToolbar"))
        .Caption(wxT("FullScreen Toolbar")).ToolbarPane().Top().LeftDockable(false).RightDockable(false));

    layoutManager.Update();
}

void EditorFrame::createStatusBar()
{
#if wxUSE_STATUSBAR
    wxCoord width[16];

    wxClientDC dc(this);
    wxFont font = dc.GetFont();
    int h;
    int num = 0;

    width[num++] = -1;
    dc.GetTextExtent(wxT(" WINDOWS-936 "), &width[num++], &h);
    dc.GetTextExtent(wxT(" Line 10000, Column 100 "), &width[num++], &h);
    dc.GetTextExtent(wxT(" Overwrite "), &width[num++], &h);
    dc.GetTextExtent(wxT(" Modified "), &width[num++], &h);
    dc.GetTextExtent(wxT(" Read/Write....."), &width[num++], &h);

    CreateStatusBar(num);
    SetStatusWidths(num, width);

#endif // wxUSE_STATUSBAR
}

void EditorFrame::_doUpdateStatusBar()
{
#if wxUSE_STATUSBAR
    if (!GetStatusBar())
        return;
    EditorPage* page = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();

    if (page && wxIsKindOf(page, TextEditorPage))
    {
        TextEditorPage* editor = static_cast<TextEditorPage*>(page);

        int panel = 0;
        int pos = editor->getControl()->GetCurrentPos();
        wxString msg;
        msg.Printf(_("Line %d, Column %d"), editor->getControl()->GetCurrentLine() + 1,
            editor->getControl()->GetColumn(pos) + 1);
        SetStatusText(editor->getFileName(), panel++);
        SetStatusText(editor->getEncodingName(), panel++);
        SetStatusText(msg, panel++);
        SetStatusText(editor->getControl()->GetOvertype() ? _("Overwrite") : _("Insert"), panel++);
        SetStatusText(editor->getIsModified() ? wxT("Modified") : wxEmptyString, panel++);
        SetStatusText(editor->getControl()->GetReadOnly() ? _("Read only") : _("Read/Write"), panel++);
    }
    else
    {
        int panel = 0;
        SetStatusText(_("Welcome to Hare Editor !"), panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(wxEmptyString, panel++);
    }
#endif // wxUSE_STATUSBAR
}

void EditorFrame::onEditorUpdateUI(EditorEvent& event)
{
    if (Manager::getInstancePtr()->getEditorPageManager() &&
        event.editorPage == Manager::getInstancePtr()->getEditorPageManager()->getActivePage())
    {
        _doUpdateStatusBar();
    }
    event.Skip();
}

void EditorFrame::onPluginAttached(EditorEvent& event)
{
    if (event.plugin)
    {
        event.plugin->buildMenuBar(GetMenuBar());

        wxAuiToolBar* toolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(16, 16),
            wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_HORZ_LAYOUT);

        if (event.plugin->buildToolBar(toolBar))
        {
            toolBar->Realize();
            toolBar->SetInitialSize();

            const PluginInfo* info = Manager::getInstancePtr()->getPluginManager()->getPluginInfo(event.plugin);

            static int next_row = 1;
            layoutManager.AddPane(toolBar, wxAuiPaneInfo().Name(info->name + wxT("Toolbar"))
                .Caption(info->title + _(" Toolbar")).ToolbarPane().Top().Row(next_row++)
                .LeftDockable(false).RightDockable(false));

            layoutManager.Update();

            wxMenuBar* mbar = GetMenuBar();
            wxMenuItem* item = mbar->FindItem(idViewToolbars, 0);
            if (item)
            {
                wxMenu* menu = item->GetSubMenu();
                if (menu)
                {
                    wxString name = info->title + _(" Toolbar");
                    if (wxNOT_FOUND == menu->FindItem(name))
                    {
                        int newId = wxNewId();
                        wxMenuItem* newItem = menu->Append(newId, name, wxEmptyString);
                        Connect(newId, wxEVT_COMMAND_MENU_SELECTED,
                            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&EditorFrame::onShowToolBar);
                        pluginToolBars[newId] = toolBar;
                    }
                }
            }
        }
        else
            delete toolBar;
    }
}

void EditorFrame::onShowToolBar(wxCommandEvent& event)
{
    wxWindow* win = 0;

    if (event.GetId() == idViewExplorer)
        win = Manager::getInstancePtr()->getExplorerManager()->getNotebook();
    else if (event.GetId() == idViewToolMain)
        win = mainToolBar;
    else if (event.GetId() == idViewToolFullScreen)
        win = fullScreenToolBar;
    else
    {
       win = pluginToolBars[event.GetId()];
    }

    if (win)
    {
        layoutManager.GetPane(win).Show(true);
        layoutManager.Update();
    }
}

void EditorFrame::onToggleFullScreen(wxCommandEvent& event)
{
    ShowFullScreen(!IsFullScreen(), wxFULLSCREEN_NOTOOLBAR | wxFULLSCREEN_NOBORDER |
        wxFULLSCREEN_NOCAPTION);

    fullScreenToolBar->ToggleTool(idViewFullScreen, IsFullScreen());
    fullScreenToolBar->Realize();
}

void EditorFrame::onLayoutSwitch(EditorEvent& event)
{
    layoutManager.LoadPerspective(event.strData);
}

void EditorFrame::onLayoutQuery(EditorEvent& event)
{
    event.strData = layoutManager.SavePerspective();
}

void EditorFrame::onAddDockWindow(EditorDockEvent& event)
{
    if (Manager::isAppShuttingDown())
        return;

    if (event.window)
    {
        layoutManager.AddPane(event.window, event.info);
        layoutManager.Update();
    }
}

void EditorFrame::onDelDockWindow(EditorDockEvent& event)
{
    if (event.window)
    {
        layoutManager.DetachPane(event.window);
        layoutManager.Update();
    }
}

void EditorFrame::onShowDockWindow(EditorDockEvent& event)
{
    if (event.window)
    {
        layoutManager.GetPane(event.window).Show(event.show);
        layoutManager.Update();
    }
}

void EditorFrame::onFileMenuUpdateUI(wxUpdateUIEvent& event)
{

}

void EditorFrame::onEditMenuUpdateUI(wxUpdateUIEvent& event)
{
    if (Manager::isAppShuttingDown())
    {
        event.Skip();
        return;
    }

    wxMenuBar* mbar = GetMenuBar();

    EditorPage* curPage = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();

    bool canUndo = false;
    bool canRedo = false;
    bool canCopy = false;
    bool canPaste = false;
    bool canCut = false;

    if (curPage)
    {
        canUndo = curPage->canUndo();
        canRedo = curPage->canRedo();
        canCopy = curPage->hasSelection();
        canPaste = curPage->canPaste();
        canCut = !curPage->isReadOnly() && curPage->hasSelection();
    }

    if (event.GetId() == idEditUndo) 
    {
        event.Enable(canUndo);
    }
    else if (event.GetId() == idEditRedo)
    {
        event.Enable(canRedo);
    }
    else if (event.GetId() == idEditCut)
    {
        event.Enable(canCut);
    }
    else if (event.GetId() == idEditCopy)
    {
        event.Enable(canCopy);
    }
    else if (event.GetId() == idEditPaste)
    {
        event.Enable(canPaste);
    }
    else
        event.Skip();
}

void EditorFrame::onToolbarDropDownCreate(wxAuiToolBarEvent& event)
{
    if (event.IsDropDownClicked())
    {
        wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(event.GetEventObject());

        tb->SetToolSticky(event.GetId(), true);

        wxMenuBar* mb = GetMenuBar();

        wxMenu* menu = 0;
        wxMenuItem* item = 0;
        if (mb)
        {
            if ((item = mb->FindItem(idFileNew, 0)))
                menu = item->GetSubMenu();
            if (menu)
            {
                // line up our menu with the button
                wxRect rect = tb->GetToolRect(event.GetId());
                wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
                pt = ScreenToClient(pt);

                PopupMenu(menu, pt);
            }
        }
        // make sure the button is "un-stuck"
        tb->SetToolSticky(event.GetId(), false);
    }
}

void EditorFrame::onToolbarDropDownDebug(wxAuiToolBarEvent& event)
{
    if (event.IsDropDownClicked())
    {
        wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(event.GetEventObject());

        tb->SetToolSticky(event.GetId(), true);

        wxRect rect = tb->GetToolRect(event.GetId());
        wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());

        wxMenu menuPopup;
        // PopupMenu

        // make sure the button is "un-stuck"
        tb->SetToolSticky(event.GetId(), false);
    }
}

void EditorFrame::onEraseBackground(wxEraseEvent& event)
{
    // for flicker-free display
    event.Skip();
}

bool EditorFrame::onDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
    wxBusyCursor useless;
    wxPaintEvent e;
    ProcessEvent(e);

    bool success = false;

    for (unsigned int i = 0; i < filenames.GetCount(); ++i)
        success &= (0 != Manager::getInstancePtr()->getEditorPageManager()->openInTextEditor(filenames[i]));

    _doUpdateStatusBar();

    return success;
}

void EditorFrame::onSize(wxSizeEvent& event)
{
    // for flicker-free display
    event.Skip();
}

void EditorFrame::onApplicationClose(wxCloseEvent& event)
{
    EditorEvent evt(editorEVT_APP_BEFORE_SHUTDOWN);
    Manager::getInstancePtr()->processEvent(evt);

    wxString layout = layoutManager.SavePerspective();
    if (!layout.IsEmpty())
        Manager::getInstancePtr()->getConfigManager()->getAppConfigFile()->setLayout(layout);

    Manager::getInstancePtr()->getExplorerManager()->getProjectExplorer()->saveWorkspace();

    layoutManager.UnInit();

    Manager::shutdown();
    wxFlatNotebook::CleanUp();
    
    Destroy();
}

void EditorFrame::onFileNew(wxCommandEvent& event)
{
    int id = event.GetId();
}

void EditorFrame::onFileOpen(wxCommandEvent& event)
{
    _doOpenFile();
}
void EditorFrame::onFileReopenProject(wxCommandEvent& event)
{
}
void EditorFrame::onFileOpenRecentProjectClearHistory(wxCommandEvent& event)
{
}
void EditorFrame::onFileReopen(wxCommandEvent& event)
{
}
void EditorFrame::onFileOpenRecentClearHistory(wxCommandEvent& event)
{
}
void EditorFrame::onFileSave(wxCommandEvent& event)
{
    EditorPage* curPage = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();
    if (curPage)
    {
        curPage->save();
    }
}
void EditorFrame::onFileSaveAll(wxCommandEvent& event)
{
}
void EditorFrame::onFileQuit(wxCommandEvent& event)
{
    Close(true);
}
void EditorFrame::onEditUndo(wxCommandEvent& event)
{
    EditorPage* curPage = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();
    if (curPage)
    {
        curPage->undo();
    }
}
void EditorFrame::onEditRedo(wxCommandEvent& event)
{
    EditorPage* curPage = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();
    if (curPage)
    {
        curPage->redo();
    }
}
void EditorFrame::onEditCut(wxCommandEvent& event)
{
    EditorPage* curPage = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();
    if (curPage)
    {
        curPage->cut();
    }
}
void EditorFrame::onEditCopy(wxCommandEvent& event)
{
    EditorPage* curPage = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();
    if (curPage)
    {
        curPage->copy();
    }
}
void EditorFrame::onEditPaste(wxCommandEvent& event)
{
    EditorPage* curPage = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();
    if (curPage)
    {
        curPage->paste();
    }
}
void EditorFrame::onEditSelectAll(wxCommandEvent& event)
{
    EditorPage* curPage = Manager::getInstancePtr()->getEditorPageManager()->getActivePage();
    if (curPage)
    {
        curPage->selectAll();
    }
}
void EditorFrame::onEditFind(wxCommandEvent& event)
{
    findReplaceDlg->Center();
    findReplaceDlg->Show();
    findReplaceDlg->SetFocus();
}
void EditorFrame::onEditFindInFile(wxCommandEvent& event)
{

}
void EditorFrame::onEditGoto(wxCommandEvent& event)
{

}
void EditorFrame::onDebugStart(wxCommandEvent& event)
{
    Manager* man = Manager::getInstancePtr();
    ProjectExplorer* pe = man->getExplorerManager()->getProjectExplorer();
    Project* prj = pe->getActiveProject();
    if (prj)
    {
        prj->debuggerName = "LuaDebugger";

        if (!prj->debuggerName.empty())
        {
            EditorPlugin* plugin = man->getPluginManager()->findPluginByName(wxString::FromUTF8(prj->debuggerName.c_str()));
            if (plugin && plugin->getType() == EPT_Debugger)
            {
                DebuggerPlugin* debugger = (DebuggerPlugin*)plugin;
                debugger->start();
            }
        }
    }
}
void EditorFrame::_doOpenFile()
{
    AppConfigFile* appConfig = Manager::getInstancePtr()->getConfigManager()->getAppConfigFile();

    if (!appConfig)
    {
        return;
    }

	wxFileDialog* dlg = new wxFileDialog(this, _("Open File"), appConfig->getOpenFilePath(), wxEmptyString,
		appConfig->getOpenFileFilters(), wxOPEN | wxMULTIPLE);

    dlg->SetFilterIndex(appConfig->getOpenFileFilterIndex());

    if (dlg->ShowModal() == wxID_OK)
    {
        appConfig->setOpenFileFilterIndex(dlg->GetFilterIndex());
        appConfig->setOpenFilePath(dlg->GetDirectory());
        wxArrayString files;
        dlg->GetPaths(files);
        onDropFiles(0, 0, files);
    }

    dlg->Destroy();
}

