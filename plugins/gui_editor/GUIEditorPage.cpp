#include "PCH.h"
#include "GUIEditorPage.h"

class TreeItemData : public wxTreeItemData
{
public:
    TreeItemData(Window* w) : window(w), sizer(0) {}
    TreeItemData(Sizer* s) : window(0), sizer(s) {}
    
    bool isWindow() const { return window != NULL; }
    bool isSizer() const { return sizer != NULL; }

    Window* window;
    Sizer* sizer;
};

IMPLEMENT_ABSTRACT_CLASS(GUIEditorPage, EditorPage)

BEGIN_EVENT_TABLE(GUIEditorPage, EditorPage)
    EVT_TREE_SEL_CHANGED(XRCID("idTreeView"), GUIEditorPage::onTreeItemSelected)
    EVT_TREE_ITEM_RIGHT_CLICK(XRCID("idTreeView"), GUIEditorPage::onTreeItemRightClick)
END_EVENT_TABLE()

GUIEditorPage::GUIEditorPage(wxWindow* parent)
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
    SAFE_DELETE(guiSys);
}

void GUIEditorPage::onTreeItemSelected(wxTreeEvent& event)
{
    if (event.GetItem().IsOk())
        treeCtrl->SetItemBold(event.GetItem(), true);
    if (event.GetOldItem().IsOk())
        treeCtrl->SetItemBold(event.GetOldItem(), false);
}

void GUIEditorPage::onTreeItemRightClick(wxTreeEvent& event)
{
    treeCtrl->SelectItem(event.GetItem());

    wxMenu* contexMenu = new wxMenu;
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(event.GetItem());

    if (data)
    {
        if (data->isWindow())
        {
            int nextId = windowClasses.size();
            for (size_t i = 0; i < sizerClasses.size(); ++i)
            {
                contexMenu->Append(nextId, wxT("Add ") + wxString::FromUTF8(sizerClasses[i]->className));
                nextId++;
            }
            PopupMenu(contexMenu);
        }
        else if (data->isSizer())
        {
            int nextId = 0;
            for (size_t i = 0; i < windowClasses.size(); ++i)
            {
                contexMenu->Append(nextId, wxT("Add ") + wxString::FromUTF8(windowClasses[i]->className));
                nextId++;
            }
            PopupMenu(contexMenu);
        }
    }
    else
    {
        if (!guiSys->getRoot())
        {
            int nextId = 0;
            for (size_t i = 0; i < windowClasses.size(); ++i)
            {
                contexMenu->Append(nextId, wxT("Add ") + wxString::FromUTF8(windowClasses[i]->className));
                nextId++;
            }
            PopupMenu(contexMenu);
        }
    }

    delete contexMenu;
}

void GUIEditorPage::renderScene()
{
    guiSys->render();
}

void GUIEditorPage::setWindow(Window* window)
{
    guiSys->setRoot(window);
}

void GUIEditorPage::setTheme(ThemePackage* themes)
{
    guiSys->setTheme(themes);
}

void GUIEditorPage::addWindow(Window* window)
{
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());
    
    if (!data || data->isSizer())
    {
        if (data)
            data->sizer->add(window);
        else
            guiSys->setRoot(window);

        wxTreeItemId item = treeCtrl->AppendItem(treeCtrl->GetSelection(), wxString::FromUTF8(window->getClassInfo()->className), 
            -1, -1, new TreeItemData(window));

        treeCtrl->Expand(treeCtrl->GetSelection());
        treeCtrl->SelectItem(item);
    }
}

void GUIEditorPage::addSizer(Sizer* sizer)
{
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());
    if (data && data->isWindow())
    {
        data->window->setSizer(sizer);

        wxTreeItemId item = treeCtrl->AppendItem(treeCtrl->GetSelection(), wxString::FromUTF8(sizer->getClassInfo()->className), 
            -1, -1, new TreeItemData(sizer));

        treeCtrl->Expand(treeCtrl->GetSelection());
        treeCtrl->SelectItem(item);
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
