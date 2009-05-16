#include "PCH.h"
#include "GUIEditorPage.h"

class TreeItemData : public wxTreeItemData
{
public:
    TreeItemData(Window* w) : window(w), sizer(0), item(0) {}
    TreeItemData(Sizer* s) : window(0), sizer(s), item(0) {}
    TreeItemData(SizerItem* i) : window(0), sizer(0), item(i) {}
    
    bool isWindow() const { return window != NULL; }
    bool isSizer() const { return sizer != NULL; }
    bool isSizerItem() const { return item != NULL; }

    Window* window;
    Sizer* sizer;
    SizerItem* item;
};

IMPLEMENT_ABSTRACT_CLASS(GUIEditorPage, EditorPage)

BEGIN_EVENT_TABLE(GUIEditorPage, EditorPage)
    EVT_TREE_SEL_CHANGED(XRCID("idTreeView"), GUIEditorPage::onTreeItemSelected)
    EVT_TREE_ITEM_RIGHT_CLICK(XRCID("idTreeView"), GUIEditorPage::onTreeItemRightClick)
    EVT_TREE_BEGIN_DRAG(XRCID("idTreeView"), GUIEditorPage::onTreeBeginDrag)
    EVT_TREE_END_DRAG(XRCID("idTreeView"), GUIEditorPage::onTreeEndDrag)
END_EVENT_TABLE()

GUIEditorPage::GUIEditorPage(wxWindow* parent) : isModified(false)
{
    wxXmlResource::Get()->LoadPanel(this, parent, wxT("idGUIEditorPanel"));

    scene = getHareApp()->createSceneManager();
    scene->setSceneListener(this);

    wxPanel* panel = XRCCTRL(*this, "idPanelR", wxPanel);
    canvas = new wxHareCanvas(panel);
    panel->GetSizer()->Add(canvas, 1, wxEXPAND, 0);
    canvas->getRenderWindow()->setSceneManager(scene);

    guiSys = new GUISystem();

    imageList = new wxImageList(16, 16);

    wxBitmap bmp;
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
    bmp.LoadFile(fullPath + wxT("root.png"), wxBITMAP_TYPE_PNG);
    rootImage = imageList->Add(bmp);
    bmp.LoadFile(fullPath + wxT("sizer.png"), wxBITMAP_TYPE_PNG);
    sizerImage = imageList->Add(bmp);
    bmp.LoadFile(fullPath + wxT("ctrl.png"), wxBITMAP_TYPE_PNG);
    ctrlImage = imageList->Add(bmp);

    treeCtrl = XRCCTRL(*this, "idTreeView", wxTreeCtrl);
    treeCtrl->SetImageList(imageList);
    treeCtrl->AddRoot(wxT("Root"), rootImage, rootImage);

    setTitle(wxT("[GUIEditor]"));

    Layout();

    Window::CLASS_INFO.findSubs(windowClasses);
    Sizer::CLASS_INFO.findSubs(sizerClasses);
    
    Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GUIEditorPage::onMenuSelected), 0, this);
}

GUIEditorPage::~GUIEditorPage()
{
    treeCtrl->SetImageList(NULL);
    SAFE_DELETE(imageList);
}

void GUIEditorPage::onTreeBeginDrag(wxTreeEvent& event)
{
    if (event.GetItem() == treeCtrl->GetRootItem())
    {
        return;
    }

    draggedItem = event.GetItem();
    event.Allow();
}

void GUIEditorPage::onTreeEndDrag(wxTreeEvent& event)
{
    bool copy = ::wxGetKeyState(WXK_CONTROL);

    wxTreeItemId itemSrc = draggedItem, itemDst = event.GetItem();

    // ensure that itemDst is not itemSrc or a child of itemSrc
    wxTreeItemId item = itemDst;
    while (item.IsOk())
    {
        if (item == itemSrc)
        {
            return;
        }
        item = treeCtrl->GetItemParent(item);
    }

    PObjectBase objSrc = GetObjectFromTreeItem( itemSrc );
    if ( !objSrc )
    {
        return;
    }

    PObjectBase objDst = GetObjectFromTreeItem( itemDst );
    if ( !objDst )
    {
        return;
    }

    // backup clipboard
    PObjectBase clipboard = AppData()->GetClipboardObject();

    // set object to clipboard
    if ( copy )
    {
        AppData()->CopyObject( objSrc );
    }
    else
    {
        AppData()->CutObject( objSrc );
    }

    if ( !AppData()->PasteObject( objDst ) && !copy )
    {
        AppData()->Undo();
    }

    AppData()->SetClipboardObject( clipboard );


}

void GUIEditorPage::onTreeItemSelected(wxTreeEvent& event)
{
    if (event.GetItem().IsOk())
        treeCtrl->SetItemBold(event.GetItem(), true);
    if (event.GetOldItem().IsOk())
        treeCtrl->SetItemBold(event.GetOldItem(), false);

    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(event.GetItem());
    Object* object = NULL;

    if (data)
    {
        if (data->isWindow())
            object = data->window;
        else if (data->isSizer())
            object = data->sizer;
        else if (data->isSizerItem())
            object = data->item;
    }
    else
        object = guiSys;

    if (object)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("Properity"), object);
    }
}

void GUIEditorPage::onTreeItemRightClick(wxTreeEvent& event)
{
    treeCtrl->SelectItem(event.GetItem());

    wxMenu* contexMenu = new wxMenu;
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(event.GetItem());

    bool add_sizer_menu = false;
    bool add_window_menu = false;

    if (data)
    {
        if (data->isWindow())
        {
            if (!data->window->getSizer())
                add_sizer_menu = true;
        }
        else if (data->isSizerItem())
        {
            if (data->item->isA(&SizerItemWindow::CLASS_INFO))
            {
                SizerItemWindow* siw = (SizerItemWindow*)data->item;
                Window* win = siw->getWindow();
                if (!win->getSizer())
                    add_sizer_menu = true;
            }
            else if (data->item->isA(&SizerItemSizer::CLASS_INFO))
            {
                add_sizer_menu = true;
                add_window_menu = true;
            }
        }
        else if (data->isSizer())
        {
            add_sizer_menu = true;
            add_window_menu = true;
        }
    }
    else
    {
        if (!guiSys->getRoot())
        {
            add_window_menu = true;
            add_sizer_menu = false;
        }
    }

    if (add_sizer_menu)
    {
        int nextId = windowClasses.size();
        for (size_t i = 0; i < sizerClasses.size(); ++i)
        {
            contexMenu->Append(nextId, wxT("Add ") + wxString::FromUTF8(sizerClasses[i]->className));
            nextId++;
        }
    }
    
    if (add_sizer_menu && add_window_menu)
        contexMenu->AppendSeparator();

    if (add_window_menu)
    {
        int nextId = 0;
        for (size_t i = 0; i < windowClasses.size(); ++i)
        {
            contexMenu->Append(nextId, wxT("Add ") + wxString::FromUTF8(windowClasses[i]->className));
            nextId++;
        }
    }

    PopupMenu(contexMenu);

    delete contexMenu;
}

void GUIEditorPage::renderScene()
{
    guiSys->render();
}

void GUIEditorPage::setWindow(Window* window)
{
    guiSys->setRoot(window);
    updateTitle();
}

void GUIEditorPage::setTheme(ThemePackage* themes)
{
    guiSys->setTheme(themes);
}

void GUIEditorPage::addWindow(Window* window)
{
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());

    Sizer* sizer = NULL;
    SizerItem* newItem = NULL;

    if (data)
    {
        if (data->isSizer())
        {
            sizer = data->sizer;
        }
        else if (data->isSizerItem())
        {
            if (data->item->isA(&SizerItemSizer::CLASS_INFO))
            {
                SizerItemSizer* sis = (SizerItemSizer*)data->item;
                sizer = sis->getSizer();
            }
        }
    }

    if (sizer)
        newItem = sizer->add(window);
    else if (!guiSys->getRoot())
        guiSys->setRoot(window);
    else
        return;

    wxTreeItemId item = treeCtrl->AppendItem(treeCtrl->GetSelection(), wxString::FromUTF8(window->getClassInfo()->className), 
        ctrlImage, ctrlImage, newItem ? new TreeItemData(newItem) : new TreeItemData(window));

    setModified(true);

    treeCtrl->Expand(treeCtrl->GetSelection());
    treeCtrl->SelectItem(item);
}

void GUIEditorPage::addSizer(Sizer* sizer)
{
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());
    if (data)
    {
        SizerItem* sizerItem = NULL;
        bool sizer_accepted = false;

        if (data->isWindow() && !data->window->getSizer())
        {
            data->window->setSizer(sizer);
            sizer_accepted = true;
        }
        else if (data->isSizer())
        {
            sizerItem = data->sizer->add(sizer);
            sizer_accepted = true;
        }
        else if (data->isSizerItem())
        {
            if (data->item->isA(&SizerItemWindow::CLASS_INFO))
            {
                SizerItemWindow* siw = (SizerItemWindow*)data->item;
                Window* win = siw->getWindow();
                if (!win->getSizer())
                {
                    win->setSizer(sizer);
                    sizer_accepted = true;
                }
            }
            else if (data->item->isA(&SizerItemSizer::CLASS_INFO))
            {
                SizerItemSizer* sis = (SizerItemSizer*)data->item;
                Sizer* sz = sis->getSizer();
                sizerItem = sz->add(sizer);
                sizer_accepted = true;
            }
        }

        if (sizer_accepted)
        {
            wxTreeItemId item = treeCtrl->AppendItem(treeCtrl->GetSelection(), wxString::FromUTF8(sizer->getClassInfo()->className), 
                sizerImage, sizerImage, sizerItem ? new TreeItemData(sizerItem) : new TreeItemData(sizer));

            setModified(true);

            treeCtrl->Expand(treeCtrl->GetSelection());
            treeCtrl->SelectItem(item);
        }
    }
}

void GUIEditorPage::onMenuSelected(wxCommandEvent& event)
{
    int id = event.GetId();

    if (id >= 0)
    {
        if (id < (int)windowClasses.size())
        {
            ClassInfo* clsInfo = windowClasses[id];
            Window::Ptr window = (Window*)clsInfo->createObject();

            if (window)
                addWindow(window);
        }
        else if (id < (int)(windowClasses.size() + sizerClasses.size()))
        {
            ClassInfo* clsInfo = sizerClasses[id - windowClasses.size()];
            Sizer::Ptr sizer = (Sizer*)clsInfo->createObject();

            if (sizer)
                addSizer(sizer);
        }
    }
}

void GUIEditorPage::updateTitle()
{
    String title = "[GUIEditor]";
    if (guiSys->getRoot()) title += guiSys->getRoot()->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}

void GUIEditorPage::setModified(bool modified)
{
    if (modified != isModified)
    {
        isModified = modified;
        updateTitle();
    }
}

bool GUIEditorPage::saveAs()
{
    bool ret = false;

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        _T("Save window layout as"),
        _T(""),
        _T(""),
        _T("Window Layout (*.layout)|*.layout|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK)
    {
        static const char* tempFileName = "/editor/~layout.temp";
        FileSystem::getSingletonPtr()->remove(tempFileName);
        guiSys->getRoot()->saveToXml(tempFileName);
        String dir = FileSystem::getSingletonPtr()->getRealDir(tempFileName);
        dir += tempFileName;
        wxString tempFile = wxString::FromUTF8(dir.c_str());
        wxFileName fName(tempFile);
        if (fName.FileExists())
        {
            wxString fileName = dlg->GetPath();
            ret = wxCopyFile(fName.GetFullPath(), fileName);
        }
    }

    dlg->Destroy();
    return ret;
}

bool GUIEditorPage::save()
{
    if (!guiSys->getRoot())
        return true;

    bool ret = false;

    if (guiSys->getRoot()->getUrl().empty())
        ret = saveAs();
    else
        ret = guiSys->getRoot()->saveToXml(guiSys->getRoot()->getUrl());

    if (ret)
        setModified(false);

    return ret;
}

bool GUIEditorPage::Show(bool show)
{
    if (show)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }
    return EditorPage::Show(show);
}