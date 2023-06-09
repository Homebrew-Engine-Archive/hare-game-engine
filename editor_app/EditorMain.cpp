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
#include <wx/xrc/xmlres.h>
#include <wx/file.h>
#include <wx/fs_zip.h>
#include <wx/fs_mem.h>

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   include <wx/harecanvas.h>
#endif

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
#include "editor.xpm"
#endif

class FileDropTarget : public wxFileDropTarget
{
public:
    FileDropTarget(EditorFrame *f) : frame(f) {}

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
    EVT_MENU(idViewToolMain, EditorFrame::onShowToolBar)
    EVT_MENU(idViewExplorer, EditorFrame::onShowToolBar)
    EVT_MENU(idViewToolFullScreen, EditorFrame::onShowToolBar)
    EVT_MENU(idViewFullScreen, EditorFrame::onToggleFullScreen)
    EVT_AUITOOLBAR_TOOL_DROPDOWN(idFileNew, EditorFrame::onToolbarDropDownCreate)
END_EVENT_TABLE()

EditorFrame::EditorFrame(wxFrame *frame, const wxString& title, const wxString& scriptDir)
 : wxFrame(frame, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE), mainToolBar(0)
{
#ifdef __WXMSW__
    SetIcon(wxICON(_EXE_ICON_));
#else
    SetIcon(wxIcon(editor));
#endif // __WXMSW__

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
    // create canvas and hide it, we mush have a window to init graphics moudle
    wxHareCanvas* canvas = new wxHareCanvas(this, wxID_ANY, true, wxPoint(0, 0), wxSize(1, 1));
    SetSizer(new wxBoxSizer(wxHORIZONTAL));
    GetSizer()->Add(canvas);
#endif
    layoutManager.SetManagedWindow(this);

    registerEvents();

    wxXmlResource::Get()->InitAllHandlers();
    wxImage::AddHandler(new wxPNGHandler);
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);

    SetDropTarget(new FileDropTarget(this));

    // Maximize and show the frame first
    Maximize(true);

    createIDE();

    Manager::getInstancePtr()->getExplorerManager()->getProjectExplorer()->loadWorkspace(scriptDir);

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
    pm->registerEvent(editorEVT_EDITOR_ACTIVATED,
        new TEventHandler<EditorFrame, EditorEvent>(this, &EditorFrame::onEditorActivated));
    pm->registerEvent(editorEVT_EDITOR_CLOSE,
        new TEventHandler<EditorFrame, EditorEvent>(this, &EditorFrame::onEditorClose));

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
    wxSize bestSize = wxSize(clientsize.GetWidth() / 4, clientsize.GetHeight() / 2);
    layoutManager.AddPane(Manager::getInstancePtr()->getExplorerManager()->getNotebook(),
        wxAuiPaneInfo().Name(wxT("ExplorerPane")).Caption(_("Explorer")).BestSize(bestSize).
        MinSize(wxSize(50, 100)).Left().CloseButton(true).MaximizeButton(true));

    wxString pluginPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_plugins/");

    Manager::getInstancePtr()->getPluginManager()->scanForPlugins(pluginPath);
    Manager::getInstancePtr()->getPluginManager()->loadAllPlugins();

    findReplaceDlg = new FindReplaceDialog(this);
    newFileDlg = new NewFileDialog(this);

    Manager::getInstancePtr()->getExplorerManager()->getNotebook()->SetDropTarget(new FileDropTarget(this));
    Manager::getInstancePtr()->getEditorPageManager()->getNotebook()->SetDropTarget(new FileDropTarget(this));

    layoutManager.Update();
}

void EditorFrame::preLoadXRC()
{
    Manager::loadResource(wxT("resources.zip"));
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

    wxString zipFile = Manager::getInstancePtr()->getAppDir() + wxT("/resources.zip#zip:");

    mainToolBar->AddTool(idFileNew, _("New"), 
        Manager::loadBitmap(zipFile + wxT("new.png")), _("New"));
    mainToolBar->SetToolDropDown(idFileNew, true);
    mainToolBar->AddTool(idFileOpen, _("Open"), 
        Manager::loadBitmap(zipFile + wxT("open.png")), _("Open"));
    mainToolBar->AddTool(idFileSave, _("Save"), 
        Manager::loadBitmap(zipFile + wxT("save.png")), _("Save"));
    mainToolBar->AddTool(idFileSaveAll, _("Save All"), 
        Manager::loadBitmap(zipFile + wxT("save_all.png")), _("Save All"));
    mainToolBar->AddSeparator();
    mainToolBar->AddTool(idEditCut, _("Cut"), 
        Manager::loadBitmap(zipFile + wxT("cut.png")), _("Cut"));
    mainToolBar->AddTool(idEditCopy, _("Copy"), 
        Manager::loadBitmap(zipFile + wxT("copy.png")), _("Copy"));
    mainToolBar->AddTool(idEditPaste, _("Paste"), 
        Manager::loadBitmap(zipFile + wxT("paste.png")), _("Paste"));
    mainToolBar->AddSeparator();
    mainToolBar->AddTool(idEditUndo, _("Undo"), 
        Manager::loadBitmap(zipFile + wxT("undo.png")), _("Undo"));
    mainToolBar->AddTool(idEditRedo, _("Redo"), 
        Manager::loadBitmap(zipFile + wxT("redo.png")), _("Redo"));
    mainToolBar->AddSeparator();
    mainToolBar->AddTool(idEditFind, _("Find"), 
        Manager::loadBitmap(zipFile + wxT("find.png")), _("Find"));
    mainToolBar->AddTool(idEditFindInFile, _("FineInFile"), 
        Manager::loadBitmap(zipFile + wxT("find_in_file.png")), _("FineInFile"));
    mainToolBar->AddSeparator();

    mainToolBar->Realize();
    mainToolBar->SetInitialSize();

    layoutManager.AddPane(mainToolBar, wxAuiPaneInfo().Name(wxT("MainToolbar"))
        .Caption(wxT("Main Toolbar")).ToolbarPane().Top().LeftDockable(false).RightDockable(false));

    fullScreenToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(16, 16),
        wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORZ_TEXT);
    fullScreenToolBar->AddTool(idViewFullScreen, _("Full Screen"), 
        Manager::loadBitmap(zipFile + wxT("full_screen.png")), wxEmptyString, wxITEM_CHECK);
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
        SetStatusText(_("Welcome to hare editor !"), panel++);
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

void EditorFrame::onEditorActivated(EditorEvent& event)
{
    _doUpdateStatusBar();
    event.Skip();
}

void EditorFrame::onEditorClose(EditorEvent& event)
{
    _doUpdateStatusBar();
    event.Skip();
}

void EditorFrame::onPluginAttached(EditorEvent& event)
{
    if (event.plugin)
    {
        const PluginInfo* info = Manager::getInstancePtr()->getPluginManager()->getPluginInfo(event.plugin);

        Log::getSingleton().logInfo("Editor plugin attached : %s", info->name.ToUTF8().data());

        event.plugin->buildMenuBar(GetMenuBar());

        wxAuiToolBar* toolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(16, 16),
            wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_HORZ_LAYOUT);

        if (event.plugin->buildToolBar(toolBar))
        {
            toolBar->Realize();
            toolBar->SetInitialSize();

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

    EditorPageManager* mgr = Manager::getInstancePtr()->getEditorPageManager();
    EditorPage* curPage = mgr->getActivePage();

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

void EditorFrame::onEraseBackground(wxEraseEvent& event)
{
    // for flicker-free display
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
    if (!EditorPageManager::getInstancePtr()->queryCloseAllPages())
    {
        event.Veto();
        return;
    }

    ExplorerManager::getInstancePtr()->removeAllProperties();

    EditorEvent evt(editorEVT_APP_BEFORE_SHUTDOWN);
    Manager::getInstancePtr()->processEvent(evt);

    wxString layout = layoutManager.SavePerspective();
    if (!layout.IsEmpty())
        Manager::getInstancePtr()->getConfigManager()->getAppConfigFile()->setLayout(layout);

    Manager::getInstancePtr()->getExplorerManager()->getProjectExplorer()->saveWorkspace();

    Hide();

    layoutManager.DetachPane(Manager::getInstancePtr()->getEditorPageManager()->getNotebook());
    layoutManager.DetachPane(Manager::getInstancePtr()->getExplorerManager()->getNotebook());
    layoutManager.DetachPane(mainToolBar);
    layoutManager.DetachPane(fullScreenToolBar);

    layoutManager.UnInit();

    Manager::shutdown();
    wxFlatNotebook::CleanUp();

    Destroy();
}

void EditorFrame::onFileNew(wxCommandEvent& event)
{
    int id = event.GetId();

    static bool firstTimeShow = true;
    if (firstTimeShow)
    {
        newFileDlg->Center();
        firstTimeShow = false;
    }
    newFileDlg->Show();
    newFileDlg->SetFocus();
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
    EditorPageManager::getInstancePtr()->saveAll();
}

void EditorFrame::onFileQuit(wxCommandEvent& event)
{
    Close(false);
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
    static bool firstTimeShow = true;
    if (firstTimeShow)
    {
        findReplaceDlg->Center();
        firstTimeShow = false;
    }
    findReplaceDlg->Show();
    findReplaceDlg->SetFocus();
}

void EditorFrame::onEditFindInFile(wxCommandEvent& event)
{
}

void EditorFrame::onEditGoto(wxCommandEvent& event)
{
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

