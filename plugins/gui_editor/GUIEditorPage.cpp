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
    canvas->Connect(wxEVT_SIZE, wxSizeEventHandler(GUIEditorPage::onSize), 0, this);
}

GUIEditorPage::~GUIEditorPage()
{
    treeCtrl->SetImageList(NULL);
    SAFE_DELETE(imageList);
}

void GUIEditorPage::onSize(wxSizeEvent& event)
{
    wxSize size = canvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        canvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
    }
    event.Skip();
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
}

void GUIEditorPage::onTreeItemSelected(wxTreeEvent& event)
{
    if (event.GetItem().IsOk())
        treeCtrl->SetItemBold(event.GetItem(), true);
    if (event.GetOldItem().IsOk())
        treeCtrl->SetItemBold(event.GetOldItem(), false);

    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(event.GetItem());
    
    if (data)
    {
        Object* object = NULL;

        if (data->isWindow())
            object = data->window;
        else if (data->isSizer())
            object = data->sizer;
        else if (data->isSizerItem())
            object = data->item;

        if (object)
        {
            Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
            Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("Properity"), object, this);
        }
    }
    else
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
        Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("Properity"), guiSys);
    }

    

    event.Skip();
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
            if (data->item->getWindow())
            {
                Window* window = data->item->getWindow();
                if (!window->getSizer())
                    add_sizer_menu = true;
            }
            else if (data->item->getSizer())
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

    event.Skip();
}

void GUIEditorPage::drawHelperRect(TreeItemData* data, uint32 color)
{
    if (data)
    {
        PointF pos(0, 0);
        SizeF size(0, 0);

        if (data->isSizer())
        {
            pos = data->sizer->getPosition();
            size = data->sizer->getSize();
        }
        else if (data->isSizerItem())
        {
            pos = data->item->getPosition();
            size = data->item->getSize();
        }
        else
            return;

        getCanvas()->setColor(color);
        getCanvas()->drawRect(pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);
        getCanvas()->setColor(0xFFFFFFFF);
    }
}

void GUIEditorPage::renderScene()
{
    guiSys->render();

    wxTreeItemId parentId = treeCtrl->GetItemParent(treeCtrl->GetSelection());
    if (parentId.IsOk())
    {
        TreeItemData* parentData = (TreeItemData*)treeCtrl->GetItemData(parentId);
        drawHelperRect(parentData, 0xFF0000FF);
    }

    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());
    drawHelperRect(data, 0xFFFF0000);
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
            sizer = data->sizer;
        else if (data->isSizerItem())
            sizer = data->item->getSizer();
    }

    if (sizer)
    {   
        newItem = sizer->add(window);

        Window* parent = sizer->getContainingWindow();

        if (parent)
        {
            window->reparent(parent);
        }
        else
            assert(false);
    }
    else if (!guiSys->getRoot())
    {
        guiSys->setRoot(window);
    }
    else
        return;

    wxTreeItemId item = treeCtrl->AppendItem(treeCtrl->GetSelection(), wxString::FromUTF8(window->getClassInfo()->className), 
        ctrlImage, ctrlImage, newItem ? new TreeItemData(newItem) : new TreeItemData(window));

    setModified(true);
    guiSys->getRoot()->layout();

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
            sizerItem = data->sizer->add(sizer, 1, uiExpand, 5);
            sizer_accepted = true;
        }
        else if (data->isSizerItem())
        {
            Window* itemWindow = data->item->getWindow();
            Sizer* itemSizer = data->item->getSizer();

            if (itemWindow && !itemWindow->getSizer())
            {
                itemWindow->setSizer(sizer);
                sizer_accepted = true;
            }
            else if (itemSizer)
            {
                sizerItem = itemSizer->add(sizer, 1, uiExpand, 5);
                sizer_accepted = true;
            }
        }

        if (sizer_accepted)
        {
            wxTreeItemId item = treeCtrl->AppendItem(treeCtrl->GetSelection(), wxString::FromUTF8(sizer->getClassInfo()->className), 
                sizerImage, sizerImage, sizerItem ? new TreeItemData(sizerItem) : new TreeItemData(sizer));

            setModified(true);

            guiSys->getRoot()->layout();

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